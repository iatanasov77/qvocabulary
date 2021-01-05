/*
 * This Class is used to import another QVocabulary Database
 * -------------------------------------------------------------
 * This needed when a new database structure is needed
 * to can import old data from old Database.
 */
#ifndef IMPORT_QVOCABULARY_H
#define IMPORT_QVOCABULARY_H

#include "Import.h"

#include "Entity/VocabularyGroup.h"

class QVocabulary : public Import
{
	public:
		static bool importFromDb( QString dbName );

	private:
		static bool parseMeta();
		static bool parseWords();

		static QSqlDatabase db;
};

#endif	// IMPORT_QVOCABULARY_H
