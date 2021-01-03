#ifndef VS_SETTINGS_H
#define VS_SETTINGS_H

#include <QSettings>

class VsSettings
{
	private:
		VsSettings();
        static VsSettings* createInstance();

        static VsSettings* _instance;
        QSettings* _settings;

	public:
        ~VsSettings();
        static VsSettings* instance();
        QSettings* settings();
        QMap<QString, QString> languages();
};

#endif // VS_SETTINGS_H
