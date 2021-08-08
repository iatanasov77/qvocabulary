#ifndef QVOCABULARY_SETTINGS_H
#define QVOCABULARY_SETTINGS_H

#include "VsSettings.h"

class QVocabularySettings : public VsSettings
{
	public:
		static QVocabularySettings* instance();

        QMap<QString, QVariant> speakerSettings();
        QMap<QString, QVariant> quizSettings();
        QMap<QString, QVariant> importSettings();
};

#endif // QVOCABULARY_SETTINGS_H
