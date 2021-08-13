#ifndef IMPORT_H
#define IMPORT_H

#include "Entity/VocabularyGroup.h"

class Import
{
	public:
		virtual ~Import();
		static bool importFromFile( QString file );
		static bool importFromDb( QString dbName );

	protected:
		static QMap<QString, QVariant> importSettings;

		static VocabularyGroupPtr createGroup( QString groupName = "Imported" );

	private:
		Import();
};

#endif	// IMPORT_H
