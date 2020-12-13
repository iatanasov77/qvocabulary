#include "Entity/Vocabulary.h"
#include <QxOrm_Impl.h>

QX_REGISTER_CPP_QX_VOCABULARY(Vocabulary)

namespace qx {
	template <> void register_class( QxClass<Vocabulary> & t )
	{
		t.id( & Vocabulary::id, "id" );
		t.data( & Vocabulary::language_1, "language_1" );
		t.data( & Vocabulary::language_2, "language_2" );

		t.data( & Vocabulary::group_id, "group_id" );	// To can Move Groups( I dont know how to do this with related object)
		//t.relationManyToOne( & Vocabulary::group, "group_id" );
	}
}

Vocabulary::Vocabulary() : id(0)
{

}

Vocabulary::~Vocabulary()
{

}
