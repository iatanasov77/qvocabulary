#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"

#include <QDebug>
#include <QTreeWidgetItem>

#include "AbstractSettingsWidget.h"
#include "SettingsWidgetGeneral.h"
#include "SettingsWidgetSpeaker.h"

SettingsWindow::SettingsWindow( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::SettingsWindow )
{
    ui->setupUi( this );
    setWindowIcon( QIcon( ":/Resources/icons/settings.svg" ) );

    // you can try with QSplitter::setStretchFactor or QSplitter::setSizes to resize the splitter.
	//ui->splitter->setStretchFactor( 2, 8 );
	ui->splitter->setSizes( QList<int>() << 200 << 700 );

    initSettingsMenu();
    initWidgets();
    showSettingsGeneral();

	connect( ui->btnApply, SIGNAL( released() ), this, SLOT( applySettings() ) );
    connect( ui->btnSaveAndExit, SIGNAL( released() ), this, SLOT( saveAndExitSettings() ) );
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::initSettingsMenu()
{
	QTreeWidgetItem *treeItem;
	ui->treeWidget->setColumnCount( 1 );
	connect( ui->treeWidget, SIGNAL( itemClicked( QTreeWidgetItem*, int ) ), this, SLOT( showSettings( QTreeWidgetItem*, int ) ) );

	treeItem	= new QTreeWidgetItem( ui->treeWidget );
	treeItem->setText( 0, "General" );

	treeItem	= new QTreeWidgetItem( ui->treeWidget );
	treeItem->setText( 0, "Speaker" );
}

void SettingsWindow::initWidgets()
{
	widgets["General"]	= new SettingsWidgetGeneral();
	widgets["Speaker"]	= new SettingsWidgetSpeaker();

	foreach ( AbstractSettingsWidget* wdg, widgets )
		ui->mainWidget->addWidget( wdg );
}

void SettingsWindow::showSettings( QTreeWidgetItem* item, int column )
{
	// Switch does not support strings
	if ( item->text( 0 ) == "General" ) {
		showSettingsGeneral();
	} else if ( item->text( 0 ) == "Speaker" ) {
		showSettingsSpeaker();
	} else {
		showSettingsUnimplemented( item->text( 0 ) );
	}
}

void SettingsWindow::showSettingsUnimplemented( QString settingsTitle )
{
	qDebug() << "Settings Unimpemented";
}

void SettingsWindow::showSettingsGeneral()
{
	ui->settingsTitle->setText( tr( "General" ) );
	ui->mainWidget->setCurrentWidget( widgets["General"] );
}

void SettingsWindow::showSettingsSpeaker()
{
	ui->settingsTitle->setText( tr( "Speaker" ) );
	ui->mainWidget->setCurrentWidget( widgets["Speaker"] );
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

void SettingsWindow::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}
