#include "ArchiveGroup.h"
#include <QxOrm_Impl.h>

QX_REGISTER_CPP_QX_VOCABULARY(ArchiveGroup)

namespace qx {
	template <> void register_class( QxClass<ArchiveGroup> & t )
	{
		t.id( & ArchiveGroup::id, "id" );
		t.data( & ArchiveGroup::name, "name" );
	}
}

ArchiveGroup::ArchiveGroup() : id(0)
{

}

ArchiveGroup::~ArchiveGroup()
{

}
