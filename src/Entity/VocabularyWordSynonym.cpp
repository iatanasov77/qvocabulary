#include "VocabularyWordSynonym.h"
#include <QxOrm_Impl.h>

QX_REGISTER_CPP_QX_VOCABULARY(VocabularyWordSynonym)

namespace qx {
	template <> void register_class( QxClass<VocabularyWordSynonym> & t )
	{
		t.id( & VocabularyWordSynonym::id, "id" );

		t.data( & VocabularyWordSynonym::word_id, "word_id" );
		t.data( & VocabularyWordSynonym::synonym_id, "synonym_id" );
		t.data( & VocabularyWordSynonym::only_words, "only_words" );

		t.relationManyToOne( & VocabularyWordSynonym::word, "word_id" );
		//t.relationOneToMany( & VocabularyWordSynonym::synonym, "ListVocabularyWordSynonym", "id" );

		t.data( & VocabularyWordSynonym::target, "target" );
	}
}

VocabularyWordSynonym::VocabularyWordSynonym() : id(0)
{

}

VocabularyWordSynonym::~VocabularyWordSynonym()
{

}
