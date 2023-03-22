#ifndef DIALOG_IMPORT_ARCHIVE_H
#define DIALOG_IMPORT_ARCHIVE_H

#include <QDialog>
#include <QSqlDatabase>
#include <QMap>

#include "Application/VsWaitingSpinner.h"

namespace Ui {
	class ImportArchiveDialog;
}

class ImportArchiveDialog : public QDialog
{
    Q_OBJECT

	public:
		explicit ImportArchiveDialog( QWidget *parent = 0 );
		~ImportArchiveDialog();

	private:
		void _importArchiveGroups( QSqlDatabase db );
		void _importArchiveWords( QSqlDatabase db );

	    Ui::ImportArchiveDialog *ui;
	    QString dbPath;
	    QMap<int, int> importedGroups;

	    VsWaitingSpinner *waitingSpinner;

	private slots:
		void importArchive();
		void setDatabase();
};

#endif // DIALOG_IMPORT_ARCHIVE_H
