#include "QuizWindow.h"
#include "ui_QuizWindow.h"

#include <QSplitter>

#include "Widget/QuizParametersWidget.h"
#include "Widget/QuizWidget.h"
#include "Entity/VocabularyMetaInfo.h"

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
}

void QuizWindow::initModels()
{
	VocabularyMetaInfoPtr metaInfo;
	metaInfo.reset( new VocabularyMetaInfo() );
	metaInfo->id = 1;
	QSqlError daoError	= qx::dao::fetch_by_id( metaInfo );
	if ( metaInfo ) {
		wdgQuiz->setViewHeader( metaInfo );
	}
}

void QuizWindow::startQuiz()
{
	wdgParameters->setEnabled( false );
	wdgQuiz->setEnabled( true );
}

void QuizWindow::stopQuiz()
{
	wdgParameters->setEnabled( true );
	wdgQuiz->setEnabled( false );
}
