#include "AddDescriptionDialog.h"
#include "ui_AddDescriptionDialog.h"

#include <QPushButton>

AddDescriptionDialog::AddDescriptionDialog( QList<QMap<QString, QString>> selectedWords, QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::AddDescriptionDialog )
{
    ui->setupUi( this );
    ui->llWord->setText( selectedWords[0]["word"] );
    ui->ltDescription->setText( selectedWords[0]["description"] );

    ui->buttonBox->button( QDialogButtonBox::Save )->setText( tr( "Save" ) );
    ui->buttonBox->button( QDialogButtonBox::Cancel )->setText( tr( "Cancel" ) );
}

AddDescriptionDialog::~AddDescriptionDialog()
{
    delete ui;
}

QString AddDescriptionDialog::getDescription()
{
	return ui->ltDescription->toPlainText();
}
