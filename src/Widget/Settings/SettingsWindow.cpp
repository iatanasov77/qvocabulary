#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"

#include <QDebug>
#include <QFile>

#include "GlobalTypes.h"
#include "GlobalTranslations.h"
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

	ui->splitter->setSizes( QList<int>() << 200 << 700 );

	initMenu();
    initWidgets();
    showSettings( ui->treeView->model()->index( 0, 0 ) );

    connect( ui->leFilter, SIGNAL( textChanged( const QString& ) ), this, SLOT( filterMenu( const QString& ) ) );
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
	widgets["General"]	= new SettingsWidgetGeneral();
	widgets["Speaker"]	= new SettingsWidgetSpeaker();

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
	TreeModel* sourceModel				= new TreeModel( file.readAll(), this, Xml );
	QSortFilterProxyModel* proxyModel	= new QSortFilterProxyModel(this);

	proxyModel->setSourceModel( sourceModel );

	ui->treeView->setModel( proxyModel );
	ui->treeView->hideColumn( 0 );	// Id Column for this model
}

void SettingsWindow::showSettings( const QModelIndex &index )
{
	QString id		= ui->treeView->model()->data( index.siblingAtColumn( 0 ) ).toString();
	QString title	= ui->treeView->model()->data( index.siblingAtColumn( 1 ) ).toString();
	//qDebug() << "TreeView ID: " << id;
	showWidget( id, title );
}

void SettingsWindow::showWidget( QString widgetId, QString widgetTitle )
{
	auto wdg	= widgets.find( widgetId );
	if( wdg != widgets.end() ) {
		ui->settingsTitle->setText( tr( qPrintable( widgetTitle ) ) );
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

void SettingsWindow::filterMenu( const QString& filterString )
{

}
