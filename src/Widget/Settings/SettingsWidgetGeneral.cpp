#include "SettingsWidgetGeneral.h"
#include "ui_SettingsWidgetGeneral.h"

#include <QWidget>
#include <QDebug>

#include "AbstractSettingsWidget.h"
#include "Application/VsApplication.h"

SettingsWidgetGeneral::SettingsWidgetGeneral( QWidget *parent ) :
	AbstractSettingsWidget( parent ),
    ui( new Ui::SettingsWidgetGeneral )
{
    ui->setupUi( this );

    init();
}

SettingsWidgetGeneral::~SettingsWidgetGeneral()
{
    delete ui;
}

void SettingsWidgetGeneral::init()
{
	ui->cmbLanguage->addItem( tr( "English" ), QVariant( "en" ) );
	ui->cmbLanguage->addItem( tr( "Bulgarian" ), QVariant( "bg" ) );
	int index = ui->cmbLanguage->findData( settings->value( "language" ).toString() );
	if ( index != -1 ) { // -1 for not found
		ui->cmbLanguage->setCurrentIndex( index );
	}

	// Init Default Locale
	QString defaultLocale = QLocale::system().name(); // e.g. "de_DE"
	defaultLocale.truncate( defaultLocale.lastIndexOf( '_' ) ); // e.g. "de"
}

void SettingsWidgetGeneral::apply()
{
	int idx						= ui->cmbLanguage->currentIndex();
	QString	selectedLanguage	= ui->cmbLanguage->itemData( idx ).toString();

	VsApplication::instance()->loadLanguage( selectedLanguage );

	settings->setValue( "language", selectedLanguage );
	settings->sync();	// Sync ini file
}

void SettingsWidgetGeneral::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}
