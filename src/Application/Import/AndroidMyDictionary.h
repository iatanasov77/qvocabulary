#ifndef IMPORT_ANDROIDMYDICTIONARY_H
#define IMPORT_ANDROIDMYDICTIONARY_H

#include "Import.h"
#include "Entity/VocabularyGroup.h"

class AndroidMyDictionary : public Import
{
	public:
		static bool importFromDb( QString dbName, QString groupName );

	private:
		static void createGroup( QString groupName );
		static bool parseWords();

		static QSqlDatabase db;
		static VocabularyGroupPtr group;
};

#endif	// IMPORT_ANDROIDMYDICTIONARY_H
