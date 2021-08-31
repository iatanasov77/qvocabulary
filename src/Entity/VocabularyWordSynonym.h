#ifndef VOCABULARY_WORD_SYNONYM_H
#define VOCABULARY_WORD_SYNONYM_H

#include "precompiled.h"
#include "Vocabulary.h"

class VocabularyWordSynonym
{
	public:
		enum EnumSynonymTarget{
	        VOCABULARY,
	        ARCHIVE
	    };

		long id;
		VocabularyPtr word;
		VocabularyPtr synonym;
		EnumSynonymTarget target;

		VocabularyWordSynonym();
		virtual ~VocabularyWordSynonym();
};

QX_REGISTER_HPP_QX_VOCABULARY(VocabularyWordSynonym, qx::trait::no_base_class_defined, 0)

typedef QSharedPointer<VocabularyWordSynonym> VocabularyWordSynonymPtr;
typedef QVector<VocabularyWordSynonymPtr> ListVocabularyWordSynonym;

#endif // VOCABULARY_WORD_SYNONYM_H
