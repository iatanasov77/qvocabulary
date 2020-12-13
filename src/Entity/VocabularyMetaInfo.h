#ifndef VOCABULARYMETAINFO_H
#define VOCABULARYMETAINFO_H

#include "precompiled.h"

class VocabularyMetaInfo
{
	public:
		VocabularyMetaInfo();
		virtual ~VocabularyMetaInfo();

		long id;
		QString name;
		QString language1;
		QString language2;
};

QX_REGISTER_HPP_QX_VOCABULARY(VocabularyMetaInfo, qx::trait::no_base_class_defined, 0)

typedef QSharedPointer<VocabularyMetaInfo> VocabularyMetaInfoPtr;
typedef QVector<VocabularyMetaInfoPtr> ListVocabularyMetaInfo;

#endif // VOCABULARYMETAINFO_H
