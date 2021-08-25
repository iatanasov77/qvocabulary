#include "SettingsWidgetQuiz.h"
#include "ui_SettingsWidgetQuiz.h"

#include <QWidget>
#include <QDebug>

#include "Application/VsApplication.h"
#include "Application/QVocabularySettings.h"
#include "AbstractSettingsWidget.h"

SettingsWidgetQuiz::SettingsWidgetQuiz( QWidget *parent ) :
	AbstractSettingsWidget( parent ),
    ui( new Ui::SettingsWidgetQuiz )
{
    ui->setupUi( this );

    initExamParameters();
    initDisplayItemColumn();
}

SettingsWidgetQuiz::~SettingsWidgetQuiz()
{
    delete ui;
}

void SettingsWidgetQuiz::initExamParameters()
{
	quizSettings	= QVocabularySettings::instance()->quizSettings();

	ui->chDisplayTranscriptions->setChecked( quizSettings["displayTranscriptions"].toBool() );
	ui->chDisplayAnswerStatus->setChecked( quizSettings["displayQuizAnswerStatus"].toBool() );
	ui->chRandomizeWords->setChecked( quizSettings["randomizeWords"].toBool() );
	ui->chDisplayTimer->setChecked( quizSettings["displayTimer"].toBool() );

	QTime timerDefaultTime( 0, 0 );
	ui->teTimerDefaultTime->setTime( timerDefaultTime.addSecs( quizSettings["timerDefaultTime"].toInt() ) );
}

void SettingsWidgetQuiz::initDisplayItemColumn()
{
	QList<QVariant> displayItemColumns	= VsSettings::instance()->value( "displayItemColumns", "Quiz" ).toList();
	for( int i = 0; i < displayItemColumns.size(); i++ ) {
		switch ( i ) {
			case 1:
				// language_1
				ui->chDisplayColumnLanguage_1->setChecked( displayItemColumns[i].toBool() );
				break;
			case 2:
				// transcription
				ui->chDisplayColumnTranscription->setChecked( displayItemColumns[i].toBool() );
				break;
			case 3:
				// language_2
				ui->chDisplayColumnTranslation->setChecked( displayItemColumns[i].toBool() );
				break;
			case 5:
				// answer
				ui->chDisplayColumnAnswer->setChecked( displayItemColumns[i].toBool() );
				break;
			case 6:
				// right_answer
				ui->chDisplayColumnIsRight->setChecked( displayItemColumns[i].toBool() );
				break;
		}
	}
}

void SettingsWidgetQuiz::apply()
{
	VsSettings *settings	= VsSettings::instance();

	settings->setValue( "displayTranscriptions", QVariant( ui->chDisplayTranscriptions->isChecked() ), "Quiz" );
	settings->setValue( "displayQuizAnswerStatus", QVariant( ui->chDisplayAnswerStatus->isChecked() ), "Quiz" );
	settings->setValue( "randomizeWords", QVariant( ui->chRandomizeWords->isChecked() ), "Quiz" );
	settings->setValue( "displayTimer", QVariant( ui->chDisplayTimer->isChecked() ), "Quiz" );

	QTime tdt				= ui->teTimerDefaultTime->time();
	int timerDefaultTime	= ( tdt.hour() * 3600 ) + ( tdt.minute() * 60 ) + tdt.second();
	settings->setValue( "timerDefaultTime", QVariant( timerDefaultTime ), "Quiz" );

	settings->setValue( "displayItemColumns", QVariant( displayItemColumns() ), "Quiz" );
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

QList<QVariant> SettingsWidgetQuiz::displayItemColumns()
{
	QList<QVariant> columns;

	columns << false;											// id
	columns << ui->chDisplayColumnLanguage_1->isChecked();		// language_1
	columns << ui->chDisplayColumnTranscription->isChecked();	// transcription
	columns << ui->chDisplayColumnTranslation->isChecked();		// language_2
	columns << false;											// quiz_id
	columns << ui->chDisplayColumnAnswer->isChecked();			// answer
	columns << ui->chDisplayColumnIsRight->isChecked();			// right_answer

	return columns;
}
