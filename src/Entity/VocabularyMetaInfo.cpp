#include "Entity/VocabularyMetaInfo.h"
#include <QxOrm_Impl.h>

QX_REGISTER_CPP_QX_VOCABULARY(VocabularyMetaInfo)

namespace qx {
	template <> void register_class( QxClass<VocabularyMetaInfo> & t )
	{
	  t.id( & VocabularyMetaInfo::id, "id" );
	  t.data( & VocabularyMetaInfo::name, "name" );
	  t.data( & VocabularyMetaInfo::language1, "language1" );
	  t.data( & VocabularyMetaInfo::language2, "language2" );
	}
}

VocabularyMetaInfo::VocabularyMetaInfo() : id(0)
{

}

VocabularyMetaInfo::~VocabularyMetaInfo()
{

}
