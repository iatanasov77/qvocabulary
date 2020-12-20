#include "QuizWindow.h"
#include "ui_QuizWindow.h"

#include <QSplitter>
#include <QMessageBox>

#include "Application/VsDatabase.h"
#include "Widget/QuizParametersWidget.h"
#include "Widget/QuizWidget.h"
#include "Entity/VocabularyMetaInfo.h"
#include "Entity/Quiz.h"

QuizWindow::QuizWindow( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::QuizWindow )
{
    ui->setupUi( this );
    setWindowIcon( QIcon( ":/Resources/icons/quiz.svg" ) );

    init();
    initConnections();
    initModels();
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
	QList<QString> groupIds;
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

	auto jsonDoc	= QJsonDocument( QJsonArray::fromVariantList( groups ) );
	quiz->groups	= jsonDoc.toJson();

	quiz->direction		= wdgParameters->getDirection();
	quiz->randomize		= wdgParameters->getChkRandomize()->isChecked();
	quiz->startedAt		= QDateTime::currentDateTime();
	daoError			= qx::dao::insert( quiz );

	wdgQuiz->setQuiz( quiz->id, groupIds, quiz->randomize, wdgParameters->time() );
	wdgQuiz->setEnabled( true );
}

void QuizWindow::stopQuiz()
{
	wdgQuiz->finishQuiz();

	wdgParameters->setEnabled( true );
	wdgQuiz->setEnabled( false );
}
