#include "QuizWidget.h"
#include "ui_QuizWidget.h"

#include <QTableView>
#include <QVector>
#include <QTimer>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Application/QVocabularySettings.h"
#include "Application/VsDatabase.h"
#include "Application/VsAssessment.h"
#include "Entity/VocabularyMetaInfo.h"
#include "Entity/Vocabulary.h"
#include "Entity/VocabularyGroup.h"

#include "ModelView/QuizViewDelegate.h"
#include "ModelView/QuizItemModelDelegate.h"
#include "ModelView/QuizItemModel.h"

QuizWidget::QuizWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::QuizWidget )
{
    ui->setupUi( this );

    hideColumns = {0, 2, 3, 4, 6};
    initModel();

    connect(
		ui->btnNextQuestion,
		SIGNAL( released() ),
		this,
		SLOT( insertWord() )
	);

    ui->frmAssessment->hide();
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
	QMap<QString, QVariant> quizSettings	= QVocabularySettings::instance()->quizSettings();

	QuizItemModelDelegate* itemDelegate	= new QuizItemModelDelegate(
		ui->tableView,
		5,
		quizSettings["displayQuizAnswerStatus"].toBool()
	);
	ui->tableView->setItemDelegateForColumn( 5, itemDelegate );

	QStringList headTitles;
	headTitles
		<< qApp->translate( "Vocabulary", "Source Language" )
		<< qApp->translate( "Vocabulary", "Transcription" )
		<< qApp->translate( "Vocabulary", "Answer" );

	metaInfo	= VsDatabase::instance()->metaInfo();

	pModelVocabulary	= new qx::QxModel<Vocabulary>();

	pModel				= new QuizItemModel();
	pModel->setHeaderData( 1, Qt::Horizontal, headTitles.at( 0 ), Qt::DisplayRole );
	pModel->setHeaderData( 2, Qt::Horizontal, headTitles.at( 1 ), Qt::DisplayRole );
	pModel->setHeaderData( 5, Qt::Horizontal, headTitles.at( 2 ), Qt::DisplayRole );

	ui->tableView->setModel( pModel );
	//ui->tableView->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
	ui->tableView->horizontalHeader()->resizeSections( QHeaderView::ResizeToContents );
	ui->tableView->horizontalHeader()->setStretchLastSection( true );
	for( int i = 0; i < hideColumns.size(); i++ ) {
		ui->tableView->hideColumn( hideColumns[i] );
	}

	connect(
		pModel,
		SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ),
		this,
		SLOT( onDataChanged( const QModelIndex&, const QModelIndex& ) )
	);

	connect(
		pModel,
		SIGNAL( rowsInserted( const QModelIndex&, int, int ) ),
		this,
		SLOT( modelRowsInserted( const QModelIndex&, int, int ) )
	);
}

void QuizWidget::setQuiz( int quizId, QList<QString> groupIds, bool randomize, bool displayTranscriptions, int time )
{
	rightAnswers		= 0;

	if ( displayTranscriptions ) {
		ui->tableView->showColumn( 2 );
	} else {
		ui->tableView->hideColumn( 2 );
	}

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

	pModel->setHeaderData( 1, Qt::Horizontal, qApp->translate( "Vocabulary", lang1.toStdString().c_str() ), Qt::DisplayRole );
	pModel->setHeaderData( 5, Qt::Horizontal, qApp->translate( "Vocabulary", lang2.toStdString().c_str() ), Qt::DisplayRole );

	if ( time > 0 ) {
		initTimer( time );
	}

	insertWord();
}

void QuizWidget::insertWord()
{
	if ( ! itemsRange.count() ) {
		emit quizFinished();
		return;
	}

	int targetRow		= pModel->rowCount( QModelIndex() );
	int randomRow		= itemsRange.takeFirst();
	int column1			= ( quiz->direction == FIRST_TO_SECOND ) ? 1 : 3;
	int column2			= ( quiz->direction == FIRST_TO_SECOND ) ? 3 : 1;

	//qDebug() << "Random Row: " << randomRow << " Column 1: " << column1 << " Column 2: " << column2;
	QVariant wordLang1			= pModelVocabulary->data( pModelVocabulary->index( randomRow, column1 ) );
	QVariant wordLang2			= pModelVocabulary->data( pModelVocabulary->index( randomRow, column2 ) );
	QVariant wordTranscription	= pModelVocabulary->data( pModelVocabulary->index( randomRow, 2 ) );

	pModel->insertRow( targetRow );
	pModel->setData( pModel->index( targetRow, 1 ), wordLang1 );
	pModel->setData( pModel->index( targetRow, 2 ), wordTranscription );
	pModel->setData( pModel->index( targetRow, 3 ), wordLang2 );
	pModel->setData( pModel->index( targetRow, 4 ), QVariant::fromValue( quiz->id ) );

	pModel->setData( pModel->index( targetRow, 6 ), false );
}

void QuizWidget::onDataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
	if ( topLeft == bottomRight && topLeft.column() == 5 ) {
		QString lang2	= pModel->data( topLeft.siblingAtColumn( 3 ) ).toString();
		QString answer	= pModel->data( topLeft.siblingAtColumn( 5 ) ).toString();

		// Detect if answer is right
		bool found	= answer.size() && lang2.contains( answer, Qt::CaseInsensitive );
		if ( found )
			rightAnswers++;

		pModel->setData( topLeft.siblingAtColumn( 6 ), found );

		if ( topLeft.row() == pModel->rowCount() -1 ) {
			insertWord();
		}
	}
}

void QuizWidget::finishQuiz()
{
	int questionsNumber	= pModel->rowCount();
	int assessment		= VsAssessment::evaluate( questionsNumber, rightAnswers );

	quiz->assessment	= assessment;
	quiz->finishedAt	= QDateTime::currentDateTime();
	QSqlError daoError	= qx::dao::update( quiz );

	disconnect( this, SIGNAL( quizFinished() ), 0, 0 );
	ui->lcdAssessment->display( assessment );
	ui->frmTimer->hide();
	ui->frmAssessment->show();

	pModel->qxSave();
}

void QuizWidget::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}

void QuizWidget::modelRowsInserted( const QModelIndex & parent, int start, int end )
{
	Q_UNUSED( parent );
	Q_UNUSED( start );
	Q_UNUSED( end );

	//qDebug() << "Row Inserted: " << start;
	//ui->tableView->scrollTo( pModel->index( start, 1 ) );
	ui->tableView->scrollToBottom();
}
