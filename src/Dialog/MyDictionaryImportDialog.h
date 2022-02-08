#ifndef DIALOG_MYDICTIONARY_IMPORT_H
#define DIALOG_MYDICTIONARY_IMPORT_H

#include <QDialog>

namespace Ui {
	class MyDictionaryImportDialog;
}

class MyDictionaryImportDialog : public QDialog
{
    Q_OBJECT

	public:
		explicit MyDictionaryImportDialog( QWidget *parent = 0 );
		~MyDictionaryImportDialog();

	private:
		QWidget *mw;	// parent MainWindow
	    Ui::MyDictionaryImportDialog *ui;

	    QString dbPath;

	private slots:
		void save();
		void setDatabase();
};

#endif // DIALOG_MYDICTIONARY_IMPORT_H
