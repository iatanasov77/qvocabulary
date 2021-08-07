#ifndef ARCHIVEGROUP_H
#define ARCHIVEGROUP_H

#include "precompiled.h"

class ArchiveGroup
{
	public:
		ArchiveGroup();
		virtual ~ArchiveGroup();

		long id;
		QString name;
};

QX_REGISTER_HPP_QX_VOCABULARY(ArchiveGroup, qx::trait::no_base_class_defined, 0)

typedef QSharedPointer<ArchiveGroup> ArchiveGroupPtr;
typedef QVector<ArchiveGroupPtr> ListArchiveGroup;

#endif // ARCHIVEGROUP_H
