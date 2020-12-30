#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"

#include <QDebug>
#include <QTreeWidgetItem>

#include "SettingsWidgetGeneral.h"

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
	//qDebug() << "Settings General";
	SettingsWidgetGeneral* wdg	= new SettingsWidgetGeneral( this );

	ui->settingsTitle->setText( tr( "General" ) );
	ui->formLayout->addWidget( wdg );
}
