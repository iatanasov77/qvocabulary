#ifndef DIALOG_NEW_DATABASE_H
#define DIALOG_NEW_DATABASE_H

#include <QDialog>

namespace Ui {
	class NewDatabaseDialog;
}

class NewDatabaseDialog : public QDialog
{
    Q_OBJECT

	public:
		explicit NewDatabaseDialog( QWidget *parent = 0 );
		~NewDatabaseDialog();

		QString database();

	private:
		QWidget *mw;	// parent MainWindow
	    Ui::NewDatabaseDialog *ui;

	    QString dbPath;

	public slots:
		void save();
};

#endif // DIALOG_NEW_DATABASE_H
