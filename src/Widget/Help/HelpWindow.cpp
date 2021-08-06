#include "HelpWindow.h"
#include "ui_HelpWindow.h"

#include <QSplitter>
#include <QHelpEngine>
#include <QHelpContentWidget>
#include <QHelpIndexWidget>

#include "Widget/Help/HelpBrowser.h"

HelpWindow::HelpWindow( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::HelpWindow )
{
    ui->setupUi( this );
    setWindowIcon( QIcon( ":/Resources/icons/system-help.svg" ) );

    initHelp();
    resize( 800, 400 );
}

HelpWindow::~HelpWindow()
{
    delete ui;
}

/*
 * Make by this tutorial: https://www.walletfox.com/course/qhelpengineexample.php
 */
void HelpWindow::initHelp()
{
	QHelpEngine* helpEngine = new QHelpEngine(
									QApplication::applicationDirPath() +
									"/Help/qvocabulary.qhc"
								);

	helpEngine->setupData();

	QTabWidget* tWidget = new QTabWidget;
	tWidget->setMaximumWidth( 200 );
	tWidget->addTab( helpEngine->contentWidget(), "Contents" );
	tWidget->addTab( helpEngine->indexWidget(), "Index" );

	HelpBrowser *textViewer = new HelpBrowser( helpEngine );
	textViewer->setSource(
		QUrl( "qthelp://vankososft.qvocabulary.help/doc/index.html" )
	);

	connect(
		helpEngine->contentWidget(),
		SIGNAL( linkActivated( QUrl ) ),
		textViewer,
		SLOT( setSource( QUrl ) )
	);

	connect(
		helpEngine->indexWidget(),
		SIGNAL( linkActivated( QUrl, QString ) ),
		textViewer,
		SLOT( setSource( QUrl ) )
	);

	QSplitter *horizSplitter = new QSplitter( Qt::Horizontal );
	horizSplitter->insertWidget( 0, tWidget );
	horizSplitter->insertWidget( 1, textViewer );

	ui->layout->addWidget( horizSplitter );
}

void HelpWindow::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}
