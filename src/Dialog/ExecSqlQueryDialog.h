#ifndef DIALOG_EXEC_SQL_QUERY_H
#define DIALOG_EXEC_SQL_QUERY_H

#include <QDialog>

namespace Ui {
	class ExecSqlQueryDialog;
}

class ExecSqlQueryDialog : public QDialog
{
    Q_OBJECT

	public:
		explicit ExecSqlQueryDialog( QWidget *parent = 0 );
		~ExecSqlQueryDialog();

	private:
		QWidget *mw;	// parent MainWindow
	    Ui::ExecSqlQueryDialog *ui;

	private slots:
		void execSqlQuery();
		void closeDialog();
};

#endif // DIALOG_EXEC_SQL_QUERY_H
