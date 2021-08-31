#ifndef DIALOG_ADD_TO_ARCHIVE_H
#define DIALOG_ADD_TO_ARCHIVE_H

#include <QDialog>
#include <QSqlDatabase>

#include "Entity/ArchiveWord.h"
#include "Widget/MultiSelectComboBox.h"

namespace Ui {
	class SynonymsDialog;
}

class SynonymsDialog : public QDialog
{
    Q_OBJECT

	public:
		explicit SynonymsDialog( int wordId, QWidget *parent = 0 );
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

		QList<int> selectedVocabularyWords();
		QList<int> selectedArchiveWords();
		void saveVocabularySynonyms();
		void saveArchiveSynonyms();

		int _wordId;
		QWidget *vww;	// parent VocabularyWordsWidget
	    Ui::SynonymsDialog *ui;
	    MultiSelectComboBox *cmbVocabulary;
	    MultiSelectComboBox *cmbArchive;

	private slots:
		void saveSynonyms();
};

#endif // DIALOG_ADD_TO_ARCHIVE_H
