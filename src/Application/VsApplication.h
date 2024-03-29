#ifndef VS_APPLICATION_H
#define VS_APPLICATION_H

#include <QDir>
#include <QTranslator>

class VsApplication
{
	private:
		VsApplication();
	    static VsApplication* createInstance();

	    void switchTranslator( QTranslator& translator, const QString& filename );

	    static VsApplication* _instance;
	    QTranslator m_translator; // contains the translations for this application
		QTranslator m_translatorQt; // contains the translations for qt
		QString m_currLang; // contains the currently loaded language
		QString m_langPath; // Path of language files. This is always fixed to /languages.

	public:
	    static VsApplication* instance();

		static QDir dataPath();
		static QString appVersion();
		static QString appBuildTime();
		static bool canOpenDb( QString dbVersion );

		QMap<QString, QString> languages();
		void loadLanguage( const QString& rLanguage );
		QString appAboutBody();
		QString quizHelpDataBody();

		QMap<QString, int> dbTableSqlLines();
		static const QString DB_VERSION;
};

#endif // VS_APPLICATION_H
