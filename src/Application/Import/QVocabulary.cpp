/*
 * This Class is used to import another QVocabulary Database
 * -------------------------------------------------------------
 * This needed when a new database structure is needed
 * to can import old data from old Database.
 */
#include "QVocabulary.h"

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Application/VsDatabase.h"
#include "Entity/VocabularyMetaInfo.h"
#include "Entity/VocabularyGroup.h"
#include "Entity/Vocabulary.h"

QSqlDatabase QVocabulary::db;

bool QVocabulary::importFromDb( QString dbName )
{
	qDebug() << "Import DB: " << dbName;
	db	= QSqlDatabase::addDatabase( "QSQLITE", "import_source" );
	db.setDatabaseName( dbName );
	db.setHostName( "localhost" );
	db.setUserName( "root" );
	db.setPassword( "root" );

	qDebug() << "Import Started.";
	if ( db.open() ) {
		qDebug() << "DB is Opened.";
		parseMeta();
		parseWords();

		db.close();
	} else {
		qDebug() << "Import Source Database Cannot Be Opened!";
	}

	return true;
}

bool QVocabulary::parseMeta()
{
	QSqlQuery query( "SELECT * FROM VocabularyMetaInfo", QVocabulary::db );
	query.next();

	// Update VocabularyMetaInfo
	VocabularyMetaInfoPtr metaInfo	= VsDatabase::instance()->metaInfo();
	metaInfo->name			= query.value( "name" ).toString();
	metaInfo->language1		= query.value( "language1" ).toString();
	metaInfo->language2		= query.value( "language2" ).toString();

	QSqlError daoError	= qx::dao::update( metaInfo );

	return true;
}

bool QVocabulary::parseWords()
{
	QSqlError daoError;
	VocabularyPtr voc;
	VocabularyGroupPtr vocg;
	int currentGroup	= -1;

	QSqlQuery query( "SELECT * FROM Vocabulary", db );
	while ( query.next() ) {
		if ( currentGroup != query.value( "group_id" ).toInt() ) {
			QSqlQuery queryGroup( QString( "SELECT * FROM VocabularyGroup WHERE id = %1" ).arg( query.value( "group_id" ).toString() ), db );
			queryGroup.next();
			vocg	= createGroup( "Imported " + queryGroup.value( "name" ).toString() );
			currentGroup	= vocg->id;
		}

		voc				= VocabularyPtr( new Vocabulary() );

		voc->group_id		= vocg->id;	// To can Move Groups( I dont know how to do this with related object)

		voc->language_1		= query.value( "language_1" ).toString();
		voc->transcription	= query.value( "transcription" ).toString();
		voc->language_2		= query.value( "language_2" ).toString();
		voc->description	= QString( "" );

		daoError		= qx::dao::insert( voc );
	}

	return true;
}
