#include "Entity/Vocabulary.h"
#include "Entity/VocabularyWordSynonym.h"
#include <QxOrm_Impl.h>

QX_REGISTER_CPP_QX_VOCABULARY(Vocabulary)
//QX_PERSISTABLE_CPP(Vocabulary)

namespace qx
{
	template <> void register_class( QxClass<Vocabulary> & t )
	{
		t.id( & Vocabulary::id, "id" );
		t.data( & Vocabulary::language_1, "language_1" );
		t.data( & Vocabulary::transcription, "transcription" );
		t.data( & Vocabulary::language_2, "language_2" );

		t.data( & Vocabulary::group_id, "group_id" );	// To can Move Groups( I dont know how to do this with related object)
		//t.relationManyToOne( & Vocabulary::group, "group_id" );

		t.data( & Vocabulary::description, "description" );

		t.relationOneToMany( & Vocabulary::synonyms, "ListVocabularyWordSynonym", "id" );

		//t.fct_0<QString>( std::mem_fn( & Vocabulary::showSynonyms ), "showSynonyms" );
		//t.data( & qx::QxClassX::invoke( "Vocabulary", "showSynonyms", VocabularyPtr voc ) );
		//t.data( & Vocabulary::synonyms, "synonyms" );
		t.data( & Vocabulary::synonyms_conactenated, "synonyms_conactenated", 0, true, false );	// https://www.qxorm.com/qxorm_en/manual.html#manual_3050
	}
}

Vocabulary::Vocabulary() : id(0)
{

}

Vocabulary::~Vocabulary()
{

}

QString Vocabulary::showSynonyms() const
{
    return "Synonyms";
}
