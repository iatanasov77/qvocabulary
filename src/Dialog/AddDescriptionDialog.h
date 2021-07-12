#ifndef DIALOG_ADD_DESCRIPTION_H
#define DIALOG_ADD_DESCRIPTION_H

#include <QWidget>
#include <QDialog>

namespace Ui {
    class AddDescriptionDialog;
}

class AddDescriptionDialog : public QDialog
{
    Q_OBJECT

	public:
		explicit AddDescriptionDialog( QString selectedWord, QWidget *parent = 0 );
		~AddDescriptionDialog();

		QString getDescription();

	private:
		Ui::AddDescriptionDialog* ui;
};

#endif // DIALOG_ADD_DESCRIPTION_H
