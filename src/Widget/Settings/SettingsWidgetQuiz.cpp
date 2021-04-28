#include "SettingsWidgetQuiz.h"
#include "ui_SettingsWidgetQuiz.h"

#include <QWidget>
#include <QDebug>

#include "Application/VsApplication.h"
#include "Application/VsSettings.h"
#include "AbstractSettingsWidget.h"

SettingsWidgetQuiz::SettingsWidgetQuiz( QWidget *parent ) :
	AbstractSettingsWidget( parent ),
    ui( new Ui::SettingsWidgetQuiz )
{
    ui->setupUi( this );

    init();
}

SettingsWidgetQuiz::~SettingsWidgetQuiz()
{
    delete ui;
}

void SettingsWidgetQuiz::init()
{
	quizSettings	= VsSettings::instance()->quizSettings();

	ui->chDisplayTranscriptions->setChecked( quizSettings["displayTranscriptions"].toBool() );
	ui->chDisplayAnswerStatus->setChecked( quizSettings["displayQuizAnswerStatus"].toBool() );
	ui->chRandomizeWords->setChecked( quizSettings["randomizeWords"].toBool() );
	ui->chDisplayTimer->setChecked( quizSettings["displayTimer"].toBool() );

	QTime timerDefaultTime( 0, 0 );
	ui->teTimerDefaultTime->setTime( timerDefaultTime.addSecs( quizSettings["timerDefaultTime"].toInt() ) );
}

void SettingsWidgetQuiz::apply()
{
	VsSettings::instance()->setValue( "displayTranscriptions", QVariant( ui->chDisplayTranscriptions->isChecked() ), "Quiz" );
	VsSettings::instance()->setValue( "displayQuizAnswerStatus", QVariant( ui->chDisplayAnswerStatus->isChecked() ), "Quiz" );
	VsSettings::instance()->setValue( "randomizeWords", QVariant( ui->chRandomizeWords->isChecked() ), "Quiz" );
	VsSettings::instance()->setValue( "displayTimer", QVariant( ui->chDisplayTimer->isChecked() ), "Quiz" );

	QTime tdt				= ui->teTimerDefaultTime->time();
	int timerDefaultTime	= ( tdt.hour() * 3600 ) + ( tdt.minute() * 60 ) + tdt.second();
	VsSettings::instance()->setValue( "timerDefaultTime", QVariant( timerDefaultTime ), "Quiz" );
}

void SettingsWidgetQuiz::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}

