#include "VsSettings.h"

#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>

#include "VsApplication.h"

/*
 * This one did the trick!
 * ------------------------
 * But I want to do this: https://stackoverflow.com/questions/46172607/qt-singleton-implementation
 */
VsSettings* VsSettings::_instance = 0;

VsSettings::VsSettings()
{
	QDir userAppDataPath	= VsApplication::dataPath();
	QString settingsFile	= userAppDataPath.absoluteFilePath( "settings.ini" );

	//_settings	= new QSettings( settingsFile, QSettings::NativeFormat );
	_settings	= new QSettings( settingsFile, QSettings::IniFormat );
}

VsSettings* VsSettings::createInstance()
{
    return new VsSettings();
}

VsSettings::~VsSettings()
{
	_settings->sync();
}

VsSettings* VsSettings::instance() {
	if ( ! _instance ) {
		_instance = createInstance();
	}

	return _instance;
	//return Singleton<VsSettings>::instance( VsSettings::createInstance() );
}

QSettings* VsSettings::settings()
{
	return _settings;
}

QVariant VsSettings::value( QString key, QString group, QVariant defaultValue )
{
	_settings->beginGroup( group );
	QVariant value	= _settings->value( key, defaultValue );
	_settings->endGroup();

	return value;
}

void VsSettings::setValue( QString key, QVariant value, QString group )
{
	_settings->beginGroup( group );
	_settings->setValue( key, value );
	_settings->endGroup();

	_settings->sync();
}

QMap<QString, QVariant> VsSettings::speakerSettings()
{
	QMap<QString, QVariant> ttsSettings;

	ttsSettings["pitch"]	= value( "pitch", "Speaker", QVariant( 0 ) );
	ttsSettings["rate"]		= value( "rate", "Speaker", QVariant( 0 ) );
	ttsSettings["volume"]	= value( "volume", "Speaker", QVariant( 70 ) );
	ttsSettings["engine"]	= value( "engine", "Speaker", QVariant( "default" ) );
	ttsSettings["language"]	= value( "language", "Speaker", QVariant( "en_US" ) );
	ttsSettings["voice"]	= value( "voice", "Speaker", QVariant( 0 ) );

	return ttsSettings;
}

QMap<QString, QVariant> VsSettings::quizSettings()
{
	QMap<QString, QVariant> quizSettings;

	quizSettings["displayQuizAnswerStatus"]	= value( "displayQuizAnswerStatus", "Quiz", QVariant( false ) );
	quizSettings["randomiizeWords"]			= value( "randomiizeWords", "Quiz", QVariant( true ) );
	quizSettings["displayTimer"]			= value( "displayTimer", "Quiz", QVariant( false ) );
	quizSettings["timerDefaultTime"]		= value( "volume", "Quiz", QVariant( 1800 ) );

	return quizSettings;
}
