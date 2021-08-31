#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "precompiled.h"
#include "VocabularyGroup.h"

#include <QSharedPointer>

class VocabularyWordSynonym;

class Vocabulary
{
	public:
		typedef QSharedPointer<VocabularyWordSynonym> VocabularyWordSynonymPtr;
	    typedef QVector<VocabularyWordSynonymPtr> ListVocabularyWordSynonym;

		Vocabulary();
		virtual ~Vocabulary();
		QString showSynonyms() const;

		long id;
		QString language_1;
		QString transcription;
		QString language_2;

		long group_id;	// To can Move Groups( I dont know how to do this with related object)
		//VocabularyGroupPtr group;

		QString description;

		ListVocabularyWordSynonym synonyms;
		QString synonyms_conactenated;
};

QX_REGISTER_PRIMARY_KEY(Vocabulary, long)
QX_REGISTER_HPP_QX_VOCABULARY(Vocabulary, qx::trait::no_base_class_defined, 0)

typedef QSharedPointer<Vocabulary> VocabularyPtr;
//typedef QVector<VocabularyPtr> ListVocabulary;
typedef qx::QxCollection<long, VocabularyPtr> ListVocabulary;

#endif // VOCABULARY_H
