#include "AddDescriptionDialog.h"
#include "ui_AddDescriptionDialog.h"

AddDescriptionDialog::AddDescriptionDialog( QString selectedWord, QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::AddDescriptionDialog )
{
    ui->setupUi( this );
    ui->llWord->setText( selectedWord );
}

AddDescriptionDialog::~AddDescriptionDialog()
{
    delete ui;
}

QString AddDescriptionDialog::getDescription()
{
	return ui->ltDescription->toPlainText();
}
