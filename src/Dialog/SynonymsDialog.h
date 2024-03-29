#ifndef DIALOG_ADD_TO_ARCHIVE_H
#define DIALOG_ADD_TO_ARCHIVE_H

#include <QDialog>
#include <QSqlDatabase>

#include "Entity/ArchiveWord.h"
#include "../lib/VankoSoft/Widget/VsMultiSelectComboBox.h"

namespace Ui {
	class SynonymsDialog;
}

class SynonymsDialog : public QDialog
{
    Q_OBJECT

	public:
		explicit SynonymsDialog( QMap<QString, QVariant> word, QWidget *parent = 0 );
		~SynonymsDialog();

	private:
		void initVocabularyCombo();
		void initVocabularyComboWithoutFilter();
		void initVocabularyComboWithFilter();
		void initArchiveCombo();
		void initArchiveComboWithoutFilter();
		void initArchiveComboWithFilter();
		QList<int> getVocabularySynonyms();
		QList<int> getArchiveSynonyms();
		QMap<QString, QVariant> getOnlyWordsSynonyms();

		QList<int> selectedVocabularyWords();
		QList<int> selectedArchiveWords();
		void saveVocabularySynonyms();
		void saveArchiveSynonyms();
		void saveOnlyWordsSynonyms();

		int _wordId;
		QWidget *vww;	// parent VocabularyWordsWidget
	    Ui::SynonymsDialog *ui;
	    VsMultiSelectComboBox *cmbVocabulary;
	    VsMultiSelectComboBox *cmbArchive;

	private slots:
		void saveSynonyms();
};

#endif // DIALOG_ADD_TO_ARCHIVE_H
