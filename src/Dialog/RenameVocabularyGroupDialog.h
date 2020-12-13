#ifndef DIALOG_NEW_VOCABULARY_GROUP_H
#define DIALOG_NEW_VOCABULARY_GROUP_H

#include <QModelIndex>
#include <QWidget>
#include <QDialog>

#include "MainWindow.h"

namespace Ui {
    class NewVocabularyGroupDialog;
}

class RenameVocabularyGroupDialog : public QDialog
{
    Q_OBJECT

	public:
		explicit RenameVocabularyGroupDialog( QWidget *parent = 0 );
		~RenameVocabularyGroupDialog();

		QString newName();

	private:
		Ui::NewVocabularyGroupDialog* ui;

};

#endif // DIALOG_NEW_VOCABULARY_GROUP_H
