#include "InitDatabaseDialog.h"
#include "ui_InitDatabaseDialog.h"

InitDatabaseDialog::InitDatabaseDialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::InitDatabaseDialog )
{
    ui->setupUi( this );
}

InitDatabaseDialog::~InitDatabaseDialog()
{
    delete ui;
}

int InitDatabaseDialog::initDatabase()
{
	if ( ui->rbNewDatabase->isChecked() ) {
		return NewDatabase;
	} else if ( ui->rbOpenDatabase->isChecked() ) {
		return OpenDatabase;
	}

	return -1;
}
