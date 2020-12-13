#ifndef DIALOG_NEW_DATABASE_H
#define DIALOG_NEW_DATABASE_H

#include "ui_NewDatabaseDialog.h"
#include <QDialog>

class NewDatabaseDialog : public QDialog
{
    Q_OBJECT

	public:
		explicit NewDatabaseDialog( QWidget *parent = 0 );
		~NewDatabaseDialog();

	private:
		QWidget *mw;	// parent MainWindow
	    Ui::NewDatabaseDialog *ui;

	public slots:
		void save();
};

#endif // DIALOG_NEW_DATABASE_H
