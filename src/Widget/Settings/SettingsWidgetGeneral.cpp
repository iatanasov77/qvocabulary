#include "SettingsWidgetGeneral.h"
#include "ui_SettingsWidgetGeneral.h"

#include <QWidget>
#include <QDebug>

#include "Application/VsApplication.h"
#include "Application/VsSettings.h"
#include "AbstractSettingsWidget.h"

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
	QMap<QString, QString> languages	= VsApplication::instance()->languages();
	foreach ( QString key, languages.keys() ) {
		ui->cmbLanguage->addItem( languages[key], QVariant( key ) );
	}

	int index = ui->cmbLanguage->findData( settings->value( "language", "General" ).toString() );
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
