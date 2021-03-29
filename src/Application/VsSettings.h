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

        QVariant value( QString key, QString group, QVariant defaultValue = QVariant() );
        void setValue( QString key, QVariant value, QString group );

        QMap<QString, QVariant> speakerSettings();
        QMap<QString, QVariant> quizSettings();
};

#endif // VS_SETTINGS_H
