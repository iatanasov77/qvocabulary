#ifndef VS_DATABASE_H
#define VS_DATABASE_H

#include "precompiled.h"
#include "Dialog/NewDatabaseDialog.h"
#include "Entity/VocabularyMetaInfo.h"

class VsDatabase
{
	private:
		static const QString DEFAULT_CONNECTION;

		VsDatabase();
        static VsDatabase* createInstance();

        void create( QString path, QString defaultGroupName = "Default Group" );
        void executeImportTransaction( QString queryStr, QSqlDatabase &db );
        void executeImportNonTransaction( QString queryStr, QSqlDatabase &db );

        static VsDatabase* _instance;
        QMap<QString, QSqlDatabase> dbConnection;
        VocabularyMetaInfoPtr metaInfoPtr;

	public:
        ~VsDatabase();
        static VsDatabase* instance();

        void connect( const QString& path, QString connection = DEFAULT_CONNECTION );
        void importSql( QFile &qf, QSqlDatabase &db );
        VocabularyMetaInfoPtr metaInfo();

        friend void NewDatabaseDialog::save();

        QString getTableCreateSql( QString tableName );
};

#endif // VS_DATABASE_H
