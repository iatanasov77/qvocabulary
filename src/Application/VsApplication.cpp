#include "VsApplication.h"

#include <QDir>
#include <QStandardPaths>

const QString VsApplication::DB_VERSION = "20201220";

QDir VsApplication::dataPath()
{
	//QDir dataPath{ QStandardPaths::writableLocation( QStandardPaths::AppDataLocation ) };
	QDir dataPath{ QStandardPaths::writableLocation( QStandardPaths::AppLocalDataLocation ) };

	//QDir dataPath{ QStandardPaths::writableLocation( QStandardPaths::GenericDataLocation ) };


	if( ! dataPath.exists() )
		dataPath.mkpath( "." );

	return dataPath;
}
