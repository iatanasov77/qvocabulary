#include "Entity/Quiz.h"
#include <QxOrm_Impl.h>

QX_REGISTER_CPP_QX_VOCABULARY(Quiz)

namespace qx {
	template <> void register_class( QxClass<Quiz> & t )
	{
		t.id( & Quiz::id, "id" );
		t.data( & Quiz::direction, "direction" );
		t.data( & Quiz::randomize, "randomize" );

		t.data( & Quiz::fromVocabulary, "from_vocabulary" );
		t.data( & Quiz::groups, "groups" );

		t.data( & Quiz::assessment, "assessment" );

		t.data( & Quiz::startedAt, "started_at" );
		t.data( & Quiz::finishedAt, "finished_at" );
		//t.relationManyToOne( & Vocabulary::group, "group_id" );
	}
}

Quiz::Quiz() : id(0)
{

}

Quiz::~Quiz()
{

}
