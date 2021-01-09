#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"

#include <QDebug>
#include <QTreeWidgetItem>

#include "GlobalTypes.h"
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

    initWidgets();
    showWidget( "General" );

	connect( ui->btnApply, SIGNAL( released() ), this, SLOT( applySettings() ) );
    connect( ui->btnSaveAndExit, SIGNAL( released() ), this, SLOT( saveAndExitSettings() ) );
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::initWidgets()
{
	widgets["General"]	= new SettingsWidgetGeneral( QT_TR_NOOP( "General" ) );
	widgets["Speaker"]	= new SettingsWidgetSpeaker( QT_TR_NOOP( "Speaker" ) );

	// Init Settings Menu
	QTreeWidgetItem *treeItem;
	ui->treeWidget->setColumnCount( 1 );
	connect( ui->treeWidget, SIGNAL( itemClicked( QTreeWidgetItem*, int ) ), this, SLOT( showSettings( QTreeWidgetItem*, int ) ) );

	foreach ( AbstractSettingsWidget* wdg, widgets ) {
		ui->mainWidget->addWidget( wdg );

		// Add to Menu
		treeItem	= new QTreeWidgetItem( ui->treeWidget );
		treeItem->setText( 0, tr( qPrintable( wdg->title() ) ) );
		treeItem->setData( 0, ObserverRole, wdg->title() );
	}
}

void SettingsWindow::showSettings( QTreeWidgetItem* item, int column )
{
	QString observerData	= item->data( 0, ObserverRole ).toString();

	showWidget( observerData );
}

void SettingsWindow::showWidget( QString observerData )
{
	auto wdg	= widgets.find( observerData );
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

void SettingsWindow::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}
