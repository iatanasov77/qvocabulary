#include "VsApplication.h"

#include <QDir>
#include <QStandardPaths>

QDir VsApplication::dataPath()
{
	QDir dataPath{ QStandardPaths::writableLocation( QStandardPaths::AppDataLocation ) };
	if( ! dataPath.exists() )
		dataPath.mkpath( "." );

	return dataPath;
}
