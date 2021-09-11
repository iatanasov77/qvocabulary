#ifndef DIALOG_NEW_VOCABULARY_GROUP_H
#define DIALOG_NEW_VOCABULARY_GROUP_H

#include <QWidget>
#include <QDialog>

namespace Ui {
    class NewVocabularyGroupDialog;
}

class NewVocabularyGroupDialog : public QDialog
{
    Q_OBJECT

	public:
		explicit NewVocabularyGroupDialog( QWidget *parent = 0 );
		~NewVocabularyGroupDialog();

	public slots:
		void save();

	private:
		Ui::NewVocabularyGroupDialog* ui;
};

#endif // DIALOG_NEW_VOCABULARY_GROUP_H
