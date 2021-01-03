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

QMap<QString, QString> VsSettings::languages()
{
	QMap<QString, QString> languages;

	/*
	 *  Always translated in Bulgarian
	 *
	languages["en"]	= QObject::tr( "English" );
	languages["bg"]	= QObject::tr( "Bulgarian" );
	*/
	languages["en"]	= "English";
	languages["bg"]	= "Bulgarian";

	return languages;
}
