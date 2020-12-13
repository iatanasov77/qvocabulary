#include "VsDatabase.h"

#include "precompiled.h"
#include <QxOrm_Impl.h>

const QString VsDatabase::DEFAULT_CONNECTION = "Default";

/*
 * This one did the trick!
 * ------------------------
 * But I want to do this: https://stackoverflow.com/questions/46172607/qt-singleton-implementation
 */
VsDatabase* VsDatabase::_instance = 0;

VsDatabase::VsDatabase()
{
}

VsDatabase* VsDatabase::createInstance()
{
    return new VsDatabase();
}

VsDatabase::~VsDatabase()
{
	qx::QxSqlDatabase::closeAllDatabases();
}

VsDatabase* VsDatabase::instance() {
	if ( ! _instance ) {
		_instance = createInstance();
	}

	return _instance;
	//return Singleton<VsDatabase>::instance( VsDatabase::createInstance() );
}

void VsDatabase::connect( const QString& path, QString connection )
{
	if ( ! QFile::exists( path ) ) {
		create( path );
	}

	qx::QxSqlDatabase::closeAllDatabases();	// E tva e istinata she u eba maikata
	qx::QxSqlDatabase::getSingleton()->setDriverName( "QSQLITE" );
	qx::QxSqlDatabase::getSingleton()->setDatabaseName( path );
	qx::QxSqlDatabase::getSingleton()->setHostName( "localhost" );
	qx::QxSqlDatabase::getSingleton()->setUserName( "root" );
	qx::QxSqlDatabase::getSingleton()->setPassword( "root" );

	//dbConnection[connection]	= qx::QxSqlDatabase::getSingleton()->getDatabase();
	//dbConnection[connection].open();
}

void VsDatabase::create( QString path, QString defaultGroupName )
{
	QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" ); //not dbConnection
	db.setDatabaseName( path );
	bool ok = db.open();

	if ( ok ) {
		QSqlQuery query;

		query.exec(
			"create table VocabularyMetaInfo "
			"(id integer primary key, "
			"name varchar(255), "
			"language1 varchar(255), "
			"language2 varchar(255))"
		);

		query.exec(
			"create table Vocabulary "
			"(id integer primary key, "
			"language_1 varchar(255), "
			"language_2 varchar(255), "
			"group_id integer)"
		);

		query.exec(
			"create table VocabularyGroup "
			"(id integer primary key, "
			"name varchar(255))"
		);

		query.prepare(
			"INSERT INTO VocabularyGroup(name) \
			VALUES(:name); "
		);
		query.bindValue( ":name", defaultGroupName );
		query.exec();

		connect( path );
	}
}
