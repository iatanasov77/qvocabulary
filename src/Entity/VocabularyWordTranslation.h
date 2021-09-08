#ifndef VOCABULARY_WORD_TRANSLATION_H
#define VOCABULARY_WORD_TRANSLATION_H

#include <QPair>

#include "precompiled.h"
#include "Vocabulary.h"

class VocabularyWordTranslation
{
	public:
		enum EnumTranslationType{
			NOUN,
			ADJECTIVE,
			VERB,
			PROVERB
		};

		/*
		 * Manual: https://www.qxorm.com/qxorm_en/faq.html#faq_101
		 */
		typedef QPair<long, QString> type_composite_key;
		static QString str_composite_key() { return "author_id_0|author_id_1|author_id_2"; }

		type_composite_key id;

		long word_id;

		QString tr_word;

		QString tr_type;
		//EnumTranslationType tr_type;

		int tr_weight;

		VocabularyWordTranslation();
		virtual ~VocabularyWordTranslation();
};

QX_REGISTER_PRIMARY_KEY(VocabularyWordTranslation, VocabularyWordTranslation::type_composite_key)
QX_REGISTER_HPP_QX_VOCABULARY(VocabularyWordTranslation, qx::trait::no_base_class_defined, 0)

typedef QSharedPointer<VocabularyWordTranslation> VocabularyWordTranslationPtr;
typedef QVector<VocabularyWordTranslationPtr> ListVocabularyWordTranslation;

#endif // VOCABULARY_WORD_TRANSLATION_H
