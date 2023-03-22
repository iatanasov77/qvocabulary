#ifndef DIALOG_QVOCABULARY_IMPORT_H
#define DIALOG_QVOCABULARY_IMPORT_H

#include <QDialog>
#include "Application/VsWaitingSpinner.h"

namespace Ui {
	class QVocabularyImportDialog;
}

class QVocabularyImportDialog : public QDialog
{
    Q_OBJECT

	public:
		explicit QVocabularyImportDialog( QWidget *parent = 0 );
		~QVocabularyImportDialog();

	private:
		QWidget *mw;	// parent MainWindow
	    Ui::QVocabularyImportDialog *ui;

	    QString dbPath;
	    VsWaitingSpinner *waitingSpinner;

	private slots:
		void save();
		void setDatabase();
};

#endif // DIALOG_QVOCABULARY_IMPORT_H
