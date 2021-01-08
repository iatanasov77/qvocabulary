#include "SettingsWidgetSpeaker.h"
#include "ui_SettingsWidgetSpeaker.h"

#include <QWidget>
#include <QDebug>
#include <QTextToSpeech>
#include <QStatusBar>

#include "Application/VsSettings.h"
#include "AbstractSettingsWidget.h"
#include "Application/VsApplication.h"

SettingsWidgetSpeaker::SettingsWidgetSpeaker( QWidget *parent ) :
	AbstractSettingsWidget( parent ),
    ui( new Ui::SettingsWidgetSpeaker ),
	speeker( nullptr )
{
    ui->setupUi( this );

    init();
}

SettingsWidgetSpeaker::~SettingsWidgetSpeaker()
{
    delete ui;
}

void SettingsWidgetSpeaker::init()
{
	statusbar	= new QStatusBar();
	ui->layoutStatus->addWidget( statusbar );

	ui->cmbEngines->addItem( "Default", QString( "default" ) );
	const auto engines	= QTextToSpeech::availableEngines();

	for ( const QString &engine : engines )
		ui->cmbEngines->addItem( engine, engine );

	ui->cmbEngines->setCurrentIndex( 0 );
	engineSelected( 0 );
}

void SettingsWidgetSpeaker::apply()
{
	int idx						= ui->cmbLanguages->currentIndex();
	QString	selectedLanguage	= ui->cmbLanguages->itemData( idx ).toString();

	VsApplication::instance()->loadLanguage( selectedLanguage );
}

void SettingsWidgetSpeaker::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}

void SettingsWidgetSpeaker::stateChanged( QTextToSpeech::State state )
{
    if ( state == QTextToSpeech::Speaking ) {
        statusbar->showMessage( "Speech started..." );
    } else if ( state == QTextToSpeech::Ready )
        statusbar->showMessage( "Speech stopped...", 2000 );
    else if ( state == QTextToSpeech::Paused )
        statusbar->showMessage( "Speech paused..." );
    else
        statusbar->showMessage( "Speech error!" );

    ui->btnPause->setEnabled( state == QTextToSpeech::Speaking );
    ui->btnResume->setEnabled( state == QTextToSpeech::Paused );
    ui->btnStop->setEnabled( state == QTextToSpeech::Speaking || state == QTextToSpeech::Paused );
}

void SettingsWidgetSpeaker::engineSelected( int index )
{
    QString engineName	= ui->cmbEngines->itemData( index ).toString();
    delete speeker;
    if ( engineName == "default" )
    	speeker = new QTextToSpeech( this );
    else
    	speeker = new QTextToSpeech( engineName, this );

    disconnect(
    	ui->cmbLanguages,
		static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
		this,
		&SettingsWidgetSpeaker::languageSelected
	);
    ui->cmbLanguages->clear();

    // Populate the languages combobox before connecting its signal.
    const QVector<QLocale> locales = speeker->availableLocales();
    //const QList<QLocale> locales = speeker->availableLocales();

    QLocale current = speeker->locale();
    for ( const QLocale &locale : locales ) {
        QString name( QString( "%1 (%2)" )
                     .arg( QLocale::languageToString( locale.language() ) )
                     .arg( QLocale::countryToString( locale.country() ) )
		);
        QVariant localeVariant( locale );
        ui->cmbLanguages->addItem( name, localeVariant );
        if ( locale.name() == current.name() )
            current = locale;
    }

    setRate( ui->slRate->value() );
    setPitch( ui->slPitch->value() );
    setVolume( ui->slVolume->value() );
    connect( ui->btnStop, &QPushButton::clicked, speeker, &QTextToSpeech::stop );
    connect( ui->btnPause, &QPushButton::clicked, speeker, &QTextToSpeech::pause );
    connect( ui->btnResume, &QPushButton::clicked, speeker, &QTextToSpeech::resume );

    connect( speeker, &QTextToSpeech::stateChanged, this, &SettingsWidgetSpeaker::stateChanged );
    connect( speeker, &QTextToSpeech::localeChanged, this, &SettingsWidgetSpeaker::localeChanged );

    connect(
		ui->cmbLanguages,
		static_cast<void (QComboBox::*)(int)>( &QComboBox::currentIndexChanged ),
		this,
		&SettingsWidgetSpeaker::languageSelected
	);
    localeChanged( current );
}

void SettingsWidgetSpeaker::languageSelected( int language )
{
	QLocale locale = ui->cmbLanguages->itemData( language ).toLocale();
	speeker->setLocale( locale );
}

void SettingsWidgetSpeaker::voiceSelected( int index )
{
	speeker->setVoice( voices.at( index ) );
}

void SettingsWidgetSpeaker::speak()
{
	speeker->say( ui->pteTestText->toPlainText() );
}
void SettingsWidgetSpeaker::stop()
{
	speeker->stop();
}

void SettingsWidgetSpeaker::setRate( int rate )
{
	speeker->setRate( rate / 10.0 );
}

void SettingsWidgetSpeaker::setPitch( int pitch )
{
	speeker->setPitch( pitch / 10.0 );
}

void SettingsWidgetSpeaker::setVolume( int volume )
{
	speeker->setVolume( volume / 100.0 );
}

void SettingsWidgetSpeaker::localeChanged( const QLocale &locale )
{
    QVariant localeVariant( locale );
    ui->cmbLanguages->setCurrentIndex( ui->cmbLanguages->findData( localeVariant ) );

    disconnect(
		ui->cmbVoices,
		static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
		this,
		&SettingsWidgetSpeaker::voiceSelected
	);
    ui->cmbVoices->clear();

    voices = speeker->availableVoices();
    QVoice currentVoice = speeker->voice();
    for ( const QVoice &voice : qAsConst( voices ) ) {
    	ui->cmbVoices->addItem( QString( "%1 - %2 - %3" ).arg( voice.name() )
                          .arg( QVoice::genderName( voice.gender() ) )
                          .arg( QVoice::ageName( voice.age() ) ) );
        if ( voice.name() == currentVoice.name() )
        	ui->cmbVoices->setCurrentIndex( ui->cmbVoices->count() - 1 );
    }
    connect(
		ui->cmbVoices,
		static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
		this,
		&SettingsWidgetSpeaker::voiceSelected
	);
}
