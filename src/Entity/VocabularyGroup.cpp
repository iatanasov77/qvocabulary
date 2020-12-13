#include "VocabularyGroup.h"
#include <QxOrm_Impl.h>

QX_REGISTER_CPP_QX_VOCABULARY(VocabularyGroup)

namespace qx {
	template <> void register_class( QxClass<VocabularyGroup> & t )
	{
		t.id( & VocabularyGroup::id, "id" );
		t.data( & VocabularyGroup::name, "name" );
	}
}

VocabularyGroup::VocabularyGroup() : id(0)
{

}

VocabularyGroup::~VocabularyGroup()
{

}
