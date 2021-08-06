#ifndef DIALOG_ADD_TO_ARCHIVE_H
#define DIALOG_ADD_TO_ARCHIVE_H

#include <QDialog>

namespace Ui {
	class AddToArchiveDialog;
}

class AddToArchiveDialog : public QDialog
{
    Q_OBJECT

	public:
		explicit AddToArchiveDialog( QWidget *parent = 0 );
		~AddToArchiveDialog();

	private:
		void _addToArchive();

		QWidget *mw;	// parent MainWindow
	    Ui::AddToArchiveDialog *ui;

	    QString dbPath;

	private slots:
		void addToArchive();
		void setDatabase();
};

#endif // DIALOG_ADD_TO_ARCHIVE_H
