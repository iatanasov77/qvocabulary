#include "VsDatabase.h"

#include "precompiled.h"
#include <QxOrm_Impl.h>

#include <QRegularExpression>

#include "VsApplication.h"
#include "Entity/VocabularyMetaInfo.h"

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

	metaInfoPtr.reset( new VocabularyMetaInfo() );
	metaInfoPtr->id = 1;
	QSqlError daoError	= qx::dao::fetch_by_id( metaInfoPtr );
}

void VsDatabase::create( QString path, QString defaultGroupName )
{
	QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" ); //not dbConnection
	db.setDatabaseName( path );
	bool ok = db.open();

	if ( ok ) {
		QString fileName = QString( ":/Resources/sql/qvocabulary-%1.sql" ).arg( VsApplication::DB_VERSION );
		QFile file( fileName );

		importSql( file, db );
		connect( path );
	}
}

VocabularyMetaInfoPtr VsDatabase::metaInfo()
{
	return metaInfoPtr;
}

void VsDatabase::importSql( QFile &qf, QSqlDatabase &db ) {
    //Read query file content
    qf.open( QIODevice::ReadOnly );
    QString queryStr( qf.readAll() );
    qf.close();

    //Check if SQL Driver supports Transactions
    if( db.driver()->hasFeature(QSqlDriver::Transactions) ) {
    	executeImportTransaction( queryStr, db );

    //Sql Driver doesn't supports transaction
    } else {
    	executeImportNonTransaction( queryStr, db );
    }
}

void VsDatabase::executeImportTransaction( QString queryStr, QSqlDatabase &db )
{
	qDebug() << "Begin Transactional Import";
	QSqlQuery query;

	//Replace comments and tabs and new lines with space
	queryStr	= queryStr.replace(
						QRegularExpression(
							"(\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
							QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption
						),
						" "
					);
	//Remove waste spaces
	queryStr = queryStr.trimmed();

	//Extracting queries
	QStringList qList = queryStr.split( ';', QString::SkipEmptyParts );

	//Initialize regular expression for detecting special queries (`begin transaction` and `commit`).
	//NOTE: I used new regular expression for Qt5 as recommended by Qt documentation.
	QRegularExpression re_transaction( "\\bbegin.transaction.*", QRegularExpression::CaseInsensitiveOption );
	QRegularExpression re_commit( "\\bcommit.*", QRegularExpression::CaseInsensitiveOption );

	//Check if query file is already wrapped with a transaction
	bool isStartedWithTransaction = re_transaction.match( qList.at( 0 ) ).hasMatch();
	if( ! isStartedWithTransaction )
		db.transaction();     //<=== not wrapped with a transaction, so we wrap it with a transaction.

	//Execute each individual queries
	foreach( const QString &s, qList ) {
		if( re_transaction.match( s ).hasMatch() )    //<== detecting special query
			db.transaction();
		else if( re_commit.match( s ).hasMatch() )    //<== detecting special query
			db.commit();
		else {
			query.exec( s );                        //<== execute normal query
			if ( query.lastError().type() != QSqlError::NoError ) {
				qDebug() << query.lastError().text();
				db.rollback();                    //<== rollback the transaction if there is any problem
			}
		}
	}
	if( ! isStartedWithTransaction )
		db.commit();          //<== ... completing of wrapping with transaction

	qDebug() << "End Transactional Import";
}

void VsDatabase::executeImportNonTransaction( QString queryStr, QSqlDatabase &db )
{
	qDebug() << "Begin Non-Transactional Import";
	QSqlQuery query;

	//...so we need to remove special queries (`begin transaction` and `commit`)
	queryStr	= queryStr.replace(
						QRegularExpression(
							"(\\bbegin.transaction.*;|\\bcommit.*;|\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
							QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption
						),
						" "
					);
	queryStr	= queryStr.trimmed();

	//Execute each individual queries
	QStringList qList = queryStr.split( ';', QString::SkipEmptyParts );
	foreach( const QString &s, qList ) {
		query.exec( s );
		if ( query.lastError().type() != QSqlError::NoError )
			qDebug() << query.lastError().text();
	}

	qDebug() << "End Non-Transactional Import";
}
