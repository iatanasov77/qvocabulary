#ifndef DIALOG_ADD_TO_ARCHIVE_H
#define DIALOG_ADD_TO_ARCHIVE_H

#include <QDialog>
#include <QSqlDatabase>

#include "Entity/ArchiveWord.h"
#include "Application/VsWaitingSpinner.h"

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
		void initGroupsCombo();
		ArchiveWordPtr getArchiveWord( QString word, int archiveGroupId );
		int _createArchiveGroup( QString groupName );
		void _addToArchive( QSqlDatabase db, int archiveGroupId );

		QWidget *mw;	// parent MainWindow
	    Ui::AddToArchiveDialog *ui;

	    QString dbPath;
	    VsWaitingSpinner *waitingSpinner;

	private slots:
		void addToArchive();
		void setDatabase();
		void onGroupsComboChanged( int index );
};

#endif // DIALOG_ADD_TO_ARCHIVE_H
