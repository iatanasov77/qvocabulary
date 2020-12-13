#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "precompiled.h"
#include "VocabularyGroup.h"

class Vocabulary
{
	public:
		Vocabulary();
		virtual ~Vocabulary();

		long id;
		QString language_1;
		QString language_2;

		long group_id;	// To can Move Groups( I dont know how to do this with related object)
		//VocabularyGroupPtr group;
};

QX_REGISTER_HPP_QX_VOCABULARY(Vocabulary, qx::trait::no_base_class_defined, 0)

typedef QSharedPointer<Vocabulary> VocabularyPtr;
typedef QVector<VocabularyPtr> ListVocabulary;

#endif // VOCABULARY_H
