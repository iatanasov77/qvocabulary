#ifndef GLOBALTYPES_H
#define GLOBALTYPES_H

enum EnumDirection {
	FIRST_TO_SECOND = 0,
	SECOND_TO_FIRST = 1
};
Q_DECLARE_METATYPE(EnumDirection)

enum ItemDataRole {
	ObserverRole = 14
};
Q_DECLARE_METATYPE(ItemDataRole)

#endif	// GLOBALTYPES_H
