#ifndef VS_APPLICATION_H
#define VS_APPLICATION_H

#include <QDir>

class VsApplication
{
	public:
		static QDir dataPath();
		static QString appVersion();

		static const QString DB_VERSION;
};

#endif // VS_APPLICATION_H
