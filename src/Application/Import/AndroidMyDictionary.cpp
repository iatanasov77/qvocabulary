#include "AndroidMyDictionary.h"

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyGroup.h"
#include "Entity/Vocabulary.h"

QSqlDatabase AndroidMyDictionary::db;
VocabularyGroupPtr AndroidMyDictionary::group;

bool AndroidMyDictionary::importFromDb( QString dbName, QString groupName )
{
	db = QSqlDatabase::addDatabase( "QODBC", "import_source" );
	db.setDatabaseName( "DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + dbName );

	if ( db.open() ) {
		createGroup( groupName );
		parseWords();

		db.close();
	} else {
		return false;
	}

	return true;
}

void AndroidMyDictionary::createGroup( QString groupName )
{
	QSqlError daoError;

	group.reset( new VocabularyGroup() );
	group->name	= groupName;
	daoError	= qx::dao::insert( group );
}

bool AndroidMyDictionary::parseWords()
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
