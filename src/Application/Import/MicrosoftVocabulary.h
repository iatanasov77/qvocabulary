#include "Import.h"

#include "Entity/VocabularyGroup.h"

class MicrosoftVocabulary : public Import
{
	public:
		static bool importFromFile( QString file );
		static bool exportToFile( QString file );

	private:
		static bool parseMeta();
		static bool parseWords();

		static QDomDocument *xml;
};
