#ifndef DIALOG_ADD_DESCRIPTION_H
#define DIALOG_ADD_DESCRIPTION_H

#include <QWidget>
#include <QDialog>
#include <QList>
#include <QMap>
#include <QString>

namespace Ui {
    class AddDescriptionDialog;
}

class AddDescriptionDialog : public QDialog
{
    Q_OBJECT

	public:
		explicit AddDescriptionDialog( QList<QMap<QString, QString>> selectedWords, QWidget *parent = 0 );
		~AddDescriptionDialog();

		QString getDescription();

	private:
		Ui::AddDescriptionDialog* ui;
};

#endif // DIALOG_ADD_DESCRIPTION_H
