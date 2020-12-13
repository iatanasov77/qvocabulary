#ifndef VS_DATABASE_H
#define VS_DATABASE_H

#include "precompiled.h"
#include "Dialog/NewDatabaseDialog.h"

class VsDatabase
{
	private:
		static const QString DEFAULT_CONNECTION;

		VsDatabase();
        static VsDatabase* createInstance();

        void create( QString path, QString defaultGroupName = "Default Group" );

        static VsDatabase* _instance;
        QMap<QString, QSqlDatabase> dbConnection;

	public:
        ~VsDatabase();
        static VsDatabase* instance();

        void connect( const QString& path, QString connection = DEFAULT_CONNECTION );

        friend void NewDatabaseDialog::save();
};

#endif // VS_DATABASE_H
