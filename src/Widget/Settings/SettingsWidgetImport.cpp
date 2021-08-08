#include "SettingsWidgetImport.h"
#include "ui_SettingsWidgetImport.h"

#include <QWidget>
#include <QDebug>

#include "Application/VsApplication.h"
#include "Application/QVocabularySettings.h"
#include "AbstractSettingsWidget.h"

SettingsWidgetImport::SettingsWidgetImport( QWidget *parent ) :
	AbstractSettingsWidget( parent ),
    ui( new Ui::SettingsWidgetImport )
{
    ui->setupUi( this );

    init();
}

SettingsWidgetImport::~SettingsWidgetImport()
{
    delete ui;
}

void SettingsWidgetImport::init()
{
	importSettings	= QVocabularySettings::instance()->importSettings();

	ui->leGroupPrefix->setText( importSettings["groupPrefix"].toString() );
	//ui->chDisplayAnswerStatus->setChecked( quizSettings["displayQuizAnswerStatus"].toBool() );
}

void SettingsWidgetImport::apply()
{
	QVocabularySettings::instance()->setValue( "groupPrefix", QVariant( ui->leGroupPrefix->text() ), "Import" );
}

void SettingsWidgetImport::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}

