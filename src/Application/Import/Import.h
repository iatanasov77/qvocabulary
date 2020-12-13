#include <QtXml>

#include "Entity/VocabularyGroup.h"

class Import
{
	public:
		virtual ~Import();
		static bool importFromFile( QString file );

	protected:
		static VocabularyGroupPtr createGroup();

	private:
		Import();
};
