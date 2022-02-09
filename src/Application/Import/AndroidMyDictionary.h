#ifndef IMPORT_ANDROIDMYDICTIONARY_H
#define IMPORT_ANDROIDMYDICTIONARY_H

#include "Import.h"
#include "Entity/VocabularyGroup.h"

class AndroidMyDictionary : public Import
{
	public:
		static bool importFromDb( QString dbName, QString groupName );

	private:
		static VocabularyGroupPtr group;
		static QSqlDatabase db;
		static QFile csvFile;

		static bool importFromXls( QString dbName, QString groupName );
		static bool parseWordsFromXls();
		static bool importFromCsv( QString dbName, QString groupName );
		static bool parseWordsFromCsv();

		static void createGroup( QString groupName );
};

#endif	// IMPORT_ANDROIDMYDICTIONARY_H
