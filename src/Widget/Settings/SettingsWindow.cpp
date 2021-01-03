#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"

#include <QDebug>
#include <QTreeWidgetItem>

#include "AbstractSettingsWidget.h"
#include "SettingsWidgetGeneral.h"

class SettingsWidgetGeneral;

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
}

void SettingsWindow::showSettings( QTreeWidgetItem* item, int column )
{
	if ( item->text( 0 ) == "General" ) {
		showSettingsGeneral();
	} else {
		showSettingsUnimpemented( item->text( 0 ) );
	}
}

void SettingsWindow::showSettingsUnimpemented( QString settingsTitle )
{
	qDebug() << "Settings Unimpemented";
}

void SettingsWindow::showSettingsGeneral()
{
	wdg	= new SettingsWidgetGeneral( this );
	//wdg	= qobject_cast<AbstractSettingsWidget *>( new SettingsWidgetGeneral( this ) );

	ui->settingsTitle->setText( tr( "General" ) );
	ui->formLayout->addWidget( wdg );
}

void SettingsWindow::applySettings()
{
	wdg->apply();
}

void SettingsWindow::saveAndExitSettings()
{
	wdg->apply();
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
