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
		long word_id;
		long synonym_id;
		QString only_words;

		VocabularyPtr word;
		VocabularyPtr synonym;

		QString target;
		//EnumSynonymTarget target;

		VocabularyWordSynonym();
		virtual ~VocabularyWordSynonym();
};

QX_REGISTER_HPP_QX_VOCABULARY(VocabularyWordSynonym, qx::trait::no_base_class_defined, 0)

typedef QSharedPointer<VocabularyWordSynonym> VocabularyWordSynonymPtr;
typedef QVector<VocabularyWordSynonymPtr> ListVocabularyWordSynonym;

#endif // VOCABULARY_WORD_SYNONYM_H
