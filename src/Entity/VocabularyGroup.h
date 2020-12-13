#ifndef VOCABULARYGROUP_H
#define VOCABULARYGROUP_H

#include "precompiled.h"

class VocabularyGroup
{
	public:
		VocabularyGroup();
		virtual ~VocabularyGroup();

		long id;
		QString name;
};

QX_REGISTER_HPP_QX_VOCABULARY(VocabularyGroup, qx::trait::no_base_class_defined, 0)

typedef QSharedPointer<VocabularyGroup> VocabularyGroupPtr;
typedef QVector<VocabularyGroupPtr> ListVocabularyGroup;

#endif // VOCABULARYGROUP_H
