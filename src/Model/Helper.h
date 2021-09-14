#ifndef MODELVIEW_HELPER_H
#define MODELVIEW_HELPER_H

#include <QList>

class Helper
{
	public:
		static QMap<int, QString> getAllGroups();
		static void delay( int secs );
};

#endif // MODELVIEW_HELPER_H
