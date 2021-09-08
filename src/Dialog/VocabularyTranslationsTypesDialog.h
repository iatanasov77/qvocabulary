#ifndef DIALOG_VOCABULARY_TRANSLATIONS_TYPES_H
#define DIALOG_VOCABULARY_TRANSLATIONS_TYPES_H

#include <QWidget>
#include <QDialog>
#include <QMap>
#include <QVariant>
#include <QButtonGroup>

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

		void initTranslations( QMap<QString, QVariant> word );
		void initTypes( int countRows );
		int getCheckedId( QString trWord );
};

#endif // DIALOG_VOCABULARY_TRANSLATIONS_TYPES_H
