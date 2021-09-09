#ifndef DIALOG_VOCABULARY_TRANSLATIONS_TYPES_H
#define DIALOG_VOCABULARY_TRANSLATIONS_TYPES_H

#include <QWidget>
#include <QDialog>
#include <QMap>
#include <QVariant>
#include <QButtonGroup>
#include <QSpinBox>

namespace Ui {
    class VocabularyTranslationsTypesDialog;
}

class VocabularyTranslationsTypesDialog : public QDialog
{
    Q_OBJECT

	public:
		explicit VocabularyTranslationsTypesDialog( QMap<QString, QVariant> word, QWidget *parent = 0 );
		~VocabularyTranslationsTypesDialog();

	public slots:
		void save();

	private:
		Ui::VocabularyTranslationsTypesDialog* ui;
		QMap<QString, QVariant> _word;
		QList<QButtonGroup *> _wordTypes;
		QList<QSpinBox *> _wordWeights;
		int _defaultWeight;

		void initTranslations( QMap<QString, QVariant> word );
		void initTypes( int countRows );
		void initWeights( int countRows );
		int getCheckedId( QString trWord );
		int getWordWeight( QString trWord );
		bool checkWordTypes();
};

#endif // DIALOG_VOCABULARY_TRANSLATIONS_TYPES_H
