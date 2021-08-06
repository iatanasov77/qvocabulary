#ifndef ARCHIVE_WORD_H
#define ARCHIVE_WORD_H

#include "precompiled.h"
#include "ArchiveGroup.h"

class ArchiveWord
{
	public:
		ArchiveWord();
		virtual ~ArchiveWord();

		long id;
		QString language_1;
		QString transcription;
		QString language_2;

		long group_id;	// To can Move Groups( I dont know how to do this with related object)
		//VocabularyGroupPtr group;

		QString description;
};

QX_REGISTER_HPP_QX_VOCABULARY(ArchiveWord, qx::trait::no_base_class_defined, 0)

typedef QSharedPointer<ArchiveWord> ArchiveWordPtr;
typedef QVector<ArchiveWordPtr> ListArchiveWord;

#endif // ARCHIVE_WORD_H
