#ifndef GLOBALTYPES_H
#define GLOBALTYPES_H

enum EnumDirection {
	FIRST_TO_SECOND = 0,
	SECOND_TO_FIRST = 1
};
Q_DECLARE_METATYPE(EnumDirection)

enum EnumFromVocabulary {
	FROM_VOCABULARY	= 0,
	FROM_ARCHIVE 	= 1
};
Q_DECLARE_METATYPE(EnumFromVocabulary)

enum ItemDataRole {
	ObserverRole = 14
};
Q_DECLARE_METATYPE(ItemDataRole)

enum ItemDataSource { Txt, Xml };
Q_DECLARE_METATYPE(ItemDataSource)

#include <QMap>
#include <QString>

static QMap<QString, QString> SynonymTargets {
	{"VOCABULARY", "VOCABULARY"},
	{"ARCHIVE", "ARCHIVE"},
	{"ONLY_WORDS", "ONLY_WORDS"}
};

static QMap<QString, QString> TranslationTypes {
	{"NOUN", "NOUN"},
	{"ADJECTIVE", "ADJECTIVE"},
	{"VERB", "VERB"},
	{"PROVERB", "PROVERB"}
};

static QList<QString> TranslationTypesList { "NOUN", "ADJECTIVE", "VERB", "PROVERB" };

#endif	// GLOBALTYPES_H
