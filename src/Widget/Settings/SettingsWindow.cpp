#include "SettingsWindow.h"
#include "GlobalTranslations.h"
#include "ui_SettingsWindow.h"

#include <QDebug>
#include <QFile>

#include "GlobalTypes.h"
#include "AbstractSettingsWidget.h"
#include "SettingsWidgetGeneral.h"
#include "SettingsWidgetSpeaker.h"

#include "ModelView/SettingsMenu/TreeModel.h"

SettingsWindow::SettingsWindow( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::SettingsWindow )
{
    ui->setupUi( this );
    setWindowIcon( QIcon( ":/Resources/icons/settings.svg" ) );

    // you can try with QSplitter::setStretchFactor or QSplitter::setSizes to resize the splitter.
	//ui->splitter->setStretchFactor( 2, 8 );
	ui->splitter->setSizes( QList<int>() << 200 << 700 );

	initMenu();
    initWidgets();
    showWidget( "General" );

    connect( ui->treeView, SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( showSettings( const QModelIndex& ) ) );
	connect( ui->btnApply, SIGNAL( released() ), this, SLOT( applySettings() ) );
    connect( ui->btnSaveAndExit, SIGNAL( released() ), this, SLOT( saveAndExitSettings() ) );
    connect( ui->btnCancel, SIGNAL( released() ), this, SLOT( cancelSettings() ) );
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::initWidgets()
{
	widgets["General"]	= new SettingsWidgetGeneral( QT_TR_NOOP( "General" ) );
	widgets["Speaker"]	= new SettingsWidgetSpeaker( QT_TR_NOOP( "Speaker" ) );

	foreach ( AbstractSettingsWidget* wdg, widgets ) {
		ui->mainWidget->addWidget( wdg );
	}
}

void SettingsWindow::initMenu()
{
	/* TREE EXAMPLE
	QFile file( ":/Resources/settings_menu/menu.txt" );
	TreeModel* model	= new TreeModel( file.readAll(), this, Txt );
	*/
	QFile file( ":/Resources/settings_menu/menu.xml" );
	file.open( QIODevice::ReadOnly );
	TreeModel* model	= new TreeModel( file.readAll(), this, Xml );

	ui->treeView->setModel( model );
	ui->treeView->hideColumn( 0 );	// Id Column for this model
}

void SettingsWindow::showSettings( const QModelIndex &index )
{
	QString id	= ui->treeView->model()->data( index.siblingAtColumn( 0 ) ).toString();
	//qDebug() << "TreeView ID: " << id;
	showWidget( id );
}

void SettingsWindow::showWidget( QString widgetId )
{
	auto wdg	= widgets.find( widgetId );
	if( wdg != widgets.end() ) {
		ui->settingsTitle->setText( tr( qPrintable( wdg.value()->title() ) ) );
		ui->mainWidget->setCurrentWidget( wdg.value() );
	} else {
		qDebug() << "Settings Unimpemented";
	}
}

void SettingsWindow::applySettings()
{
	AbstractSettingsWidget*	w	= qobject_cast<AbstractSettingsWidget*>( ui->mainWidget->currentWidget() );
	w->apply();
	if ( w == widgets["Speaker"] )
		emit speakerSettingsUpdated();
}

void SettingsWindow::saveAndExitSettings()
{
	applySettings();
	close();
}

void SettingsWindow::cancelSettings()
{
	close();
}

void SettingsWindow::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}
