#include "QuizWidget.h"
#include "ui_QuizWidget.h"

#include <QTableView>
#include <QVector>
#include <QTimer>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Widget/Quiz/QuizWindow.h"

#include "Application/QVocabularySettings.h"
#include "Application/VsDatabase.h"
#include "Application/VsAssessment.h"
#include "Entity/VocabularyMetaInfo.h"
#include "Entity/Vocabulary.h"
#include "Entity/VocabularyGroup.h"
#include "Entity/ArchiveWord.h"
#include "Entity/ArchiveGroup.h"

#include "View/QuizExamTableView.h"
#include "View/ViewDelegate/Quiz/QuizViewDelegate.h"
#include "View/ViewDelegate/Quiz/QuizItemModelDelegate.h"
#include "Model/QuizItemModel.h"
#include "Model/VocabularyWordsModel.h"

QuizWidget::QuizWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::QuizWidget )
{
    ui->setupUi( this );
    qw			= parent;

    hideColumns = {0, 1, 3, 5, 6, 7, 8};
    initView();
    initModel();

    connect(
		ui->btnNextQuestion,
		SIGNAL( released() ),
		this,
		SLOT( insertWord() )
	);

    ui->btnOpenQuiz->hide();
    connect(
		ui->btnOpenQuiz,
		SIGNAL( released() ),
		this,
		SLOT( openQuiz() )
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

QPushButton* QuizWidget::btnOpenQuiz()
{
	return ui->btnOpenQuiz;
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

void QuizWidget::initView()
{
	ui->verticalLayout->removeWidget( ui->tableView );
	ui->tableView	= new QuizExamTableView( ui->frame );
	ui->verticalLayout->addWidget( ui->tableView );
}

void QuizWidget::initModel()
{
	QMap<QString, QVariant> quizSettings	= QVocabularySettings::instance()->quizSettings();

	QuizItemModelDelegate* itemDelegate	= new QuizItemModelDelegate(
		ui->tableView,
		4,
		quizSettings["displayQuizAnswerStatus"].toBool()
	);
	ui->tableView->setItemDelegateForColumn( 4, itemDelegate );

	QStringList headTitles;
	headTitles
		<< qApp->translate( "Vocabulary", "Source Language" )
		<< qApp->translate( "Vocabulary", "Answer" )
		<< qApp->translate( "Vocabulary", "Transcription" )
		<< qApp->translate( "Vocabulary", "Description" )
		<< qApp->translate( "Vocabulary", "Synonyms" );

	metaInfo	= VsDatabase::instance()->metaInfo();

	pModel				= new QuizItemModel();
	pModel->setHeaderData( 2, Qt::Horizontal, headTitles.at( 0 ), Qt::DisplayRole );
	pModel->setHeaderData( 4, Qt::Horizontal, headTitles.at( 1 ), Qt::DisplayRole );
	pModel->setHeaderData( 6, Qt::Horizontal, headTitles.at( 2 ), Qt::DisplayRole );
	pModel->setHeaderData( 7, Qt::Horizontal, headTitles.at( 3 ), Qt::DisplayRole );
	pModel->setHeaderData( 8, Qt::Horizontal, headTitles.at( 4 ), Qt::DisplayRole );

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

void QuizWidget::setQuiz( int quizId, QMap<QString, QVariant> parameters )
{
	rightAnswers				= 0;;
	QList<QVariant> groupIds	= parameters["groupIds"].toList();
	bool randomize				= parameters["randomize"].toBool();
	bool displayTranscriptions	= parameters["displayTranscriptions"].toBool();
	int time					= parameters["time"].toInt();
	int countWords				= parameters["countWords"].toInt();
	QString wordsFrom			= parameters["wordsFrom"].toString();
	if ( wordsFrom == "ArchiveWord" ) {
		pModelVocabulary	=  new qx::QxModel<ArchiveWord>();
	} else {
		pModelVocabulary	= new VocabularyWordsModel();
	}

	quiz.reset( new Quiz() );
	quiz->id 			= quizId;
	QSqlError daoError	= qx::dao::fetch_by_id( quiz );

	// Clear Previous Quiz Items
	pModel->clear();

	QString fetchGroups	= QString::number( groupIds.takeFirst().toInt() );
	foreach ( QVariant groupId, groupIds ) {
		fetchGroups += ", " + QString::number( groupId.toInt() );
	}
	QString query		= QString( "WHERE group_id IN ( %1 )" ).arg( fetchGroups );
	pModelVocabulary->qxFetchByQuery( query );
	questionsNumber	= countWords > 0 && countWords < pModelVocabulary->rowCount() ?
							countWords :
							pModelVocabulary->rowCount();
	itemsRange		= QVector<int>( questionsNumber );
	std::iota( itemsRange.begin(), itemsRange.end(), 0 );
	if ( randomize )
		std::random_shuffle( itemsRange.begin(), itemsRange.end() );

	QString lang1	= ( quiz->direction == FIRST_TO_SECOND ) ? metaInfo->language1 : metaInfo->language2;
	QString lang2	= ( quiz->direction == FIRST_TO_SECOND ) ? metaInfo->language2 : metaInfo->language1;

	pModel->setHeaderData( 2, Qt::Horizontal, qApp->translate( "Vocabulary", lang1.toStdString().c_str() ), Qt::DisplayRole );
	pModel->setHeaderData( 3, Qt::Horizontal, qApp->translate( "Vocabulary", lang2.toStdString().c_str() ), Qt::DisplayRole );

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

	// Fetch from Vocabulary
	QVariant wordLang1			= pModelVocabulary->data( pModelVocabulary->index( randomRow, column1 ) );
	QVariant wordLang2			= pModelVocabulary->data( pModelVocabulary->index( randomRow, column2 ) );

	// Set in Quiz Item
	pModel->insertRow( targetRow );
	pModel->setData( pModel->index( targetRow, 1 ), QVariant::fromValue( quiz->id ) );
	pModel->setData( pModel->index( targetRow, 2 ), wordLang1 );
	pModel->setData( pModel->index( targetRow, 3 ), wordLang2 );
	pModel->setData( pModel->index( targetRow, 5 ), false );

	insertWordHelpData( targetRow, randomRow );
}

void QuizWidget::insertWordHelpData( int quizItemRow, int vocabularyRow )
{
	// Fetch from Vocabulary
	QVariant wordTranscription	= pModelVocabulary->data( pModelVocabulary->index( vocabularyRow, 2 ) );
	QVariant wordDescription	= pModelVocabulary->data( pModelVocabulary->index( vocabularyRow, 5 ) );
	QVariant wordSynonyms		= pModelVocabulary->data( pModelVocabulary->index( vocabularyRow, 6 ) );

	// Set in Quiz Item
	pModel->setData( pModel->index( quizItemRow, 6 ), wordTranscription );
	pModel->setData( pModel->index( quizItemRow, 7 ), wordDescription );
	pModel->setData( pModel->index( quizItemRow, 8 ), wordSynonyms );
}

void QuizWidget::onDataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
	if ( topLeft == bottomRight && topLeft.column() == 4 ) {
		QString lang2	= pModel->data( topLeft.siblingAtColumn( 3 ) ).toString();
		QString answer	= pModel->data( topLeft.siblingAtColumn( 4 ) ).toString().trimmed();

		// Detect if answer is right
		bool found	= answer.size() && lang2.contains( answer, Qt::CaseInsensitive );
		if ( found )
			rightAnswers++;

		pModel->setData( topLeft.siblingAtColumn( 5 ), found );

		if ( topLeft.row() == pModel->rowCount() -1 ) {
			insertWord();
		}
	}
}

void QuizWidget::startQuiz()
{
	//qDebug() << "QuizWidget::startQuiz CALLED !!!";

	ui->tableView->setEnabled( true );
	ui->btnNextQuestion->setEnabled( true );
	ui->btnStopQuiz->setEnabled( true );
	ui->btnOpenQuiz->hide();
}

void QuizWidget::finishQuiz()
{
	int assessment		= VsAssessment::evaluate( questionsNumber, rightAnswers );

	quiz->assessment	= assessment;
	quiz->finishedAt	= QDateTime::currentDateTime();
	QSqlError daoError	= qx::dao::update( quiz );

	disconnect( this, SIGNAL( quizFinished() ), 0, 0 );
	ui->lcdAssessment->display( assessment );
	ui->frmTimer->hide();
	ui->frmAssessment->show();
	ui->lblAssesmentDetails->setText( QString( "( %1/%2 )" ).arg( rightAnswers ).arg( questionsNumber ) );

	pModel->qxSave();

	ui->tableView->setEnabled( false );
	ui->btnNextQuestion->setEnabled( false );
	ui->btnStopQuiz->setEnabled( false );

	ui->btnOpenQuiz->show();
}

void QuizWidget::openQuiz()
{
	qobject_cast<QuizWindow *>( qw )->openQuiz( quiz->id, "Opened Quiz ;)" );
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
