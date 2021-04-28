#include "Entity/QuizItem.h"
#include <QxOrm_Impl.h>

QX_REGISTER_CPP_QX_VOCABULARY(QuizItem)

namespace qx {
	template <> void register_class( QxClass<QuizItem> & t )
	{
		t.id( & QuizItem::id, "id" );
		t.data( & QuizItem::language_1, "language_1" );
		t.data( & QuizItem::transcription, "transcription" );
		t.data( & QuizItem::language_2, "language_2" );

		t.data( & QuizItem::quiz_id, "quiz_id" );	// To can Move Groups( I dont know how to do this with related object)
		//t.relationManyToOne( & Vocabulary::group, "group_id" );

		t.data( & QuizItem::answer, "answer" );
		t.data( & QuizItem::rightAnswer, "right_answer" );
	}
}

QuizItem::QuizItem() : id(0)
{

}

QuizItem::~QuizItem()
{

}
