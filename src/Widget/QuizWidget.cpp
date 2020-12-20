#include "QuizWidget.h"
#include "ui_QuizWidget.h"

#include <QTableView>
#include <QVector>
#include <QTimer>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Application/VsDatabase.h"
#include "Application/VsAssessment.h"
#include "Entity/VocabularyMetaInfo.h"
#include "Entity/Vocabulary.h"
#include "Entity/VocabularyGroup.h"

#include "ModelView/QuizViewDelegate.h"
#include "ModelView/QuizItemModel.h"

QuizWidget::QuizWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::QuizWidget )
{
    ui->setupUi( this );

    hideColumns = {0, 2, 3, 5};
    initModel();

    connect(
		ui->btnNextQuestion,
		SIGNAL( released() ),
		this,
		SLOT( insertWord() )
	);

    ui->frmTimer->hide();
}

QuizWidget::~QuizWidget()
{
    delete ui;
}

QPushButton* QuizWidget::btnStopQuiz()
{
	return ui->btnStopQuiz;
}

void QuizWidget::initTimer( int time )
{
	if ( time > 0 ) {
		timerSeconds	= time;

		QTimer *timer	= new QTimer( this );
		connect( timer, &QTimer::timeout, this, &QuizWidget::updateTimer );
		timer->start( 1000 );

		ui->frmTimer->show();
	}
}

void QuizWidget::updateTimer()
{
	if ( timerSeconds <= 0 ) {
		emit quizFinished();
		return;
	}

	timerSeconds--;

	QTime time( 0, 0 );
	QString text = time.addSecs( timerSeconds ).toString( "mm:ss" );

	ui->lcdNumber->display( text );
}

void QuizWidget::initModel()
{
	metaInfo	= VsDatabase::instance()->metaInfo();

	pModelVocabulary	= new qx::QxModel<Vocabulary>();

	pModel				= new QuizItemModel();

	ui->tableView->setModel( pModel );
	ui->tableView->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
	for( int i = 0; i < hideColumns.size(); i++ ) {
		ui->tableView->hideColumn( hideColumns[i] );
	}

	connect(
		pModel,
		SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ),
		this,
		SLOT( onDataChanged( const QModelIndex&, const QModelIndex& ) )
	);
}

void QuizWidget::setQuiz( int quizId, QList<QString> groupIds, bool randomize, int time )
{
	rightAnswers		= 0;

	quiz.reset( new Quiz() );
	quiz->id 			= quizId;
	QSqlError daoError	= qx::dao::fetch_by_id( quiz );

	// Clear Previous Quiz Items
	pModel->clear();

	QString query		= QString( "WHERE group_id IN ( %1 )" ).arg( groupIds.join( "," ) );
	pModelVocabulary->qxFetchByQuery( query );
	itemsRange	= QVector<int>( pModelVocabulary->rowCount() );
	std::iota( itemsRange.begin(), itemsRange.end(), 0 );
	if ( randomize )
		std::random_shuffle( itemsRange.begin(), itemsRange.end() );

	QString lang1	= ( quiz->direction == FIRST_TO_SECOND ) ? metaInfo->language1 : metaInfo->language2;
	QString lang2	= ( quiz->direction == FIRST_TO_SECOND ) ? metaInfo->language2 : metaInfo->language1;

	pModel->setHeaderData( 1, Qt::Horizontal, lang1, Qt::DisplayRole );
	pModel->setHeaderData( 4, Qt::Horizontal, lang2, Qt::DisplayRole );

	if ( time > 0 ) {
		initTimer( time );
	}
}

void QuizWidget::insertWord()
{
	if ( ! itemsRange.count() )
		return;

	int targetRow		= pModel->rowCount( QModelIndex() );
	int randomRow		= itemsRange.takeFirst();
	int column1			= ( quiz->direction == FIRST_TO_SECOND ) ? 1 : 2;
	int column2			= ( quiz->direction == FIRST_TO_SECOND ) ? 2 : 1;

	//qDebug() << "Random Row: " << randomRow << " Column 1: " << column1 << " Column 2: " << column2;
	QVariant wordLang1	= pModelVocabulary->data( pModelVocabulary->index( randomRow, column1 ) );
	QVariant wordLang2	= pModelVocabulary->data( pModelVocabulary->index( randomRow, column2 ) );

	pModel->insertRow( targetRow );
	pModel->setData( pModel->index( targetRow, 1 ), wordLang1 );
	pModel->setData( pModel->index( targetRow, 2 ), wordLang2 );
	pModel->setData( pModel->index( targetRow, 3 ), QVariant::fromValue( quiz->id ) );
}

void QuizWidget::onDataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
	if ( topLeft == bottomRight && topLeft.column() == 4 ) {
		QString lang2	= pModel->data( topLeft.siblingAtColumn( 2 ) ).toString();
		QString answer	= pModel->data( topLeft.siblingAtColumn( 4 ) ).toString();

		// Detect if answer is right
//		QRegExp rx( "(?i)\b" + lang2 + "\b" );
//		bool found	= answer.indexOf ( rx ) >= 0;
		bool found	= lang2.contains( answer );
		if ( found )
			rightAnswers++;

		//qDebug() << "Lang2: " << lang2 << " Answer: " << answer << " Right: " << found;
		pModel->setData( topLeft.siblingAtColumn( 5 ), found );
	}
}

void QuizWidget::finishQuiz()
{
	int questionsNumber	= pModel->rowCount();

	quiz->assessment	= VsAssessment::evaluate( questionsNumber, rightAnswers );
	quiz->finishedAt	= QDateTime::currentDateTime();
	QSqlError daoError	= qx::dao::update( quiz );

	pModel->qxSave();
}
