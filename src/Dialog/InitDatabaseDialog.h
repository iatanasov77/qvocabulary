#ifndef DIALOG_INIT_DATABASE_H
#define DIALOG_INIT_DATABASE_H

#include <QDialog>

namespace Ui {
	class InitDatabaseDialog;
}

class InitDatabaseDialog : public QDialog
{
    Q_OBJECT

	public:
		explicit InitDatabaseDialog( QWidget *parent = 0 );
		~InitDatabaseDialog();

		int initDatabase();
		static const int NewDatabase	= 0;
		static const int OpenDatabase	= 1;

	private:
	    Ui::InitDatabaseDialog *ui;
};

#endif // DIALOG_INIT_DATABASE_H
