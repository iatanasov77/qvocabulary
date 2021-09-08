#include "VocabularyWordTranslation.h"
#include <QxOrm_Impl.h>

QX_REGISTER_CPP_QX_VOCABULARY(VocabularyWordTranslation)

namespace qx {
	template <> void register_class( QxClass<VocabularyWordTranslation> & t )
	{
		/*
		 * Manual: https://www.qxorm.com/qxorm_en/faq.html#faq_101
		 */
		t.id(& VocabularyWordTranslation::id, VocabularyWordTranslation::str_composite_key());

		t.data( & VocabularyWordTranslation::word_id, "word_id" );
		t.data( & VocabularyWordTranslation::tr_word, "tr_word" );

		t.data( & VocabularyWordTranslation::tr_type, "tr_type" );
		t.data( & VocabularyWordTranslation::tr_weight, "tr_weight" );
	}
}

VocabularyWordTranslation::VocabularyWordTranslation(): id( 0, "" )
{

}

VocabularyWordTranslation::~VocabularyWordTranslation()
{

}
