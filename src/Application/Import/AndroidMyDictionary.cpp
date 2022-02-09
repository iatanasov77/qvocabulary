#include "AndroidMyDictionary.h"

#include <QMimeDatabase>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyGroup.h"
#include "Entity/Vocabulary.h"

QFile AndroidMyDictionary::csvFile;
QSqlDatabase AndroidMyDictionary::db;
VocabularyGroupPtr AndroidMyDictionary::group;

bool AndroidMyDictionary::importFromDb( QString dbName, QString groupName )
{
	QMimeDatabase db;
	QMimeType mime	= db.mimeTypeForFile( dbName );
	if ( mime.inherits( "text/plain" ) ) {
		return importFromCsv( dbName, groupName );
	} else {
		return importFromXls( dbName, groupName );
	}
}

bool AndroidMyDictionary::importFromXls( QString dbName, QString groupName )
{
	db = QSqlDatabase::addDatabase( "QODBC", "import_source" );
	db.setDatabaseName( "DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + dbName );
	if ( ! db.open() ) {
		return false;
	}

	createGroup( groupName );
	parseWordsFromXls();
	db.close();

	return true;
}

bool AndroidMyDictionary::importFromCsv( QString dbName, QString groupName )
{
	csvFile.setFileName( dbName );
	if ( ! csvFile.open( QIODevice::ReadOnly ) ) {
		return false;
	}

	createGroup( groupName );
	parseWordsFromCsv();

	return true;
}

void AndroidMyDictionary::createGroup( QString groupName )
{
	QSqlError daoError;

	group.reset( new VocabularyGroup() );
	group->name	= groupName;
	daoError	= qx::dao::insert( group );
}

bool AndroidMyDictionary::parseWordsFromXls()
{
	QSqlError daoError;
	VocabularyPtr voc;

	// Select from Sheet 'en'
	QSqlQuery query( "select * from [en$]", db ); // Select range, place A1:B5 after $
	while ( query.next() ) {
		voc				= VocabularyPtr( new Vocabulary() );

		voc->group_id		= group->id;
		voc->language_1		= query.value( 2 ).toString();
		voc->transcription	= query.value( 3 ).toString();
		voc->language_2		= query.value( 4 ).toString();

		daoError			= qx::dao::insert( voc );
	}

	return true;
}

bool AndroidMyDictionary::parseWordsFromCsv()
{
	QSqlError daoError;
	VocabularyPtr voc;
	QList<QByteArray> wordList;

	while ( ! csvFile.atEnd() ) {
		wordList	= csvFile.readLine().split( ',' );
		if (
		    wordList.size() == 7 &&
			wordList.at( 2 ) != "Word" // CSV Header
		) {
			voc				= VocabularyPtr( new Vocabulary() );

			voc->group_id		= group->id;
			voc->language_1		= QString::fromLocal8Bit( wordList.at( 2 ) );
			voc->transcription	= QString::fromLocal8Bit( wordList.at( 3 ) );
			voc->language_2		= QString::fromLocal8Bit( wordList.at( 4 ) ).trimmed().replace( ";", "," );

			daoError			= qx::dao::insert( voc );
		}
	}

	return true;
}
