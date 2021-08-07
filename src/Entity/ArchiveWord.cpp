#include "Entity/ArchiveWord.h"
#include <QxOrm_Impl.h>

QX_REGISTER_CPP_QX_VOCABULARY(ArchiveWord)

namespace qx {
	template <> void register_class( QxClass<ArchiveWord> & t )
	{
		t.id( & ArchiveWord::id, "id" );
		t.data( & ArchiveWord::language_1, "language_1" );
		t.data( & ArchiveWord::transcription, "transcription" );
		t.data( & ArchiveWord::language_2, "language_2" );

		t.data( & ArchiveWord::group_id, "group_id" );	// To can Move Groups( I dont know how to do this with related object)
		//t.relationManyToOne( & ArchiveWord::group, "group_id" );

		t.data( & ArchiveWord::description, "description" );
	}
}

ArchiveWord::ArchiveWord() : id(0)
{

}

ArchiveWord::~ArchiveWord()
{

}
