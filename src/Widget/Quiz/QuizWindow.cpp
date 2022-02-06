#include "QuizWindow.h"
#include "ui_QuizWindow.h"

#include <QSplitter>
#include <QMessageBox>

#include "MainWindow.h"
#include "Application/VsSettings.h"
#include "Application/VsDatabase.h"
#include "Widget/Quiz/QuizParametersWidget.h"
#include "Widget/Quiz/QuizWidget.h"
#include "Entity/VocabularyMetaInfo.h"
#include "Entity/Quiz.h"

QuizWindow::QuizWindow( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::QuizWindow )
{
    ui->setupUi( this );
    setWindowIcon( QIcon( ":/Resources/icons/quiz.svg" ) );
    mw	= parent;

    init();
    initConnections();
    initModels();

    // Set QuizWindow Size Previous Set In Settings
	VsSettings* settings	= VsSettings::instance();
	int qwWidth			= settings->value( "quizWindowWidth", "Quiz" ).toInt();
	int qwHeight		= settings->value( "quizWindowHeight", "Quiz" ).toInt();
	if ( qwWidth && qwHeight ) {
		resize( qwWidth, qwHeight );
	}
}

QuizWindow::~QuizWindow()
{
    delete ui;
}

void QuizWindow::init()
{
	QSplitter* horizSplitter	= new QSplitter( Qt::Horizontal );
	wdgParameters 				= new QuizParametersWidget( this );
	wdgQuiz 					= new QuizWidget( this );

	wdgQuiz->setEnabled( false );
	wdgParameters->setMaximumWidth( 300 );
	horizSplitter->insertWidget( 0, wdgParameters );
	horizSplitter->insertWidget( 1, wdgQuiz );

	ui->layout->addWidget( horizSplitter );
}

void QuizWindow::initConnections()
{
	connect(
		wdgParameters->btnStartQuiz(),
		SIGNAL( released() ),
		this,
		SLOT( startQuiz() )
	);

	connect(
		wdgQuiz->btnStopQuiz(),
		SIGNAL( released() ),
		this,
		SLOT( stopQuiz() )
	);

	connect(
		wdgQuiz,
		SIGNAL( quizFinished() ),
		this,
		SLOT( stopQuiz() )
	);
}

void QuizWindow::initModels()
{
	VocabularyMetaInfoPtr metaInfo	= VsDatabase::instance()->metaInfo();
	if ( metaInfo ) {
		wdgParameters->setMetaInfo( metaInfo );
	}
}

void QuizWindow::startQuiz()
{
	// Initialize selected groups
	QVariantList groupIds;
	QVariantList groups;
	foreach ( QCheckBox* chk, wdgParameters->getChkGroups() ) {
		if ( chk->isChecked() ) {
			groups << chk->property( "groupName" );
			groupIds << chk->property( "groupId" ).toString();
		}
	}
	if ( groups.isEmpty() ) {
		QMessageBox::warning(
			this,
			tr( "Warning" ),
			tr( "Please select at least a group!" )
		);
		return;
	}

	// Initialize Quiz
	QSqlError daoError;

	wdgParameters->setEnabled( false );

	QuizPtr quiz;
	quiz.reset( new Quiz() );

	auto jsonDoc			= QJsonDocument( QJsonArray::fromVariantList( groups ) );
	quiz->groups			= jsonDoc.toJson();

	quiz->fromVocabulary	= wdgParameters->getFromVocabulary();
	quiz->direction			= wdgParameters->getDirection();
	quiz->randomize			= wdgParameters->getChkRandomize()->isChecked();
	quiz->startedAt			= QDateTime::currentDateTime();
	daoError				= qx::dao::insert( quiz );

	bool diplayTranscriptions	= wdgParameters->getChkDisplayTranscriptions()->isChecked();

	QMap<QString, QVariant> parameters;
	parameters["groupIds"]				= groupIds;
	parameters["randomize"]				= quiz->randomize;
	parameters["diplayTranscriptions"]	= diplayTranscriptions;
	parameters["time"]					= wdgParameters->time();
	parameters["countWords"]			= wdgParameters->wordsCount();
	parameters["wordsFrom"]				= wdgParameters->wordsFrom();

	wdgQuiz->setQuiz( quiz->id, parameters );


	wdgQuiz->setEnabled( true );
	wdgQuiz->startQuiz();
}

void QuizWindow::stopQuiz()
{
	wdgQuiz->finishQuiz();

	wdgParameters->setEnabled( true );
	//wdgQuiz->setEnabled( false );
}

void QuizWindow::openQuiz( int quizId, QString quizTitle )
{
	qobject_cast<MainWindow *>( mw )->openQuiz( quizId, quizTitle );
}

void QuizWindow::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}

void QuizWindow::resizeEvent( QResizeEvent* event )
{
	QWidget::resizeEvent( event );

	VsSettings* settings	= VsSettings::instance();
	QSize qwSize		= event->size(); // Returns the new size of the widget. This is the same as QWidget::size().

	settings->setValue( "quizWindowWidth", QVariant( qwSize.width() ), "Quiz" );
	settings->setValue( "quizWindowHeight", QVariant( qwSize.height() ), "Quiz" );
}
