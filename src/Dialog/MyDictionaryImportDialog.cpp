#include "MyDictionaryImportDialog.h"
#include "ui_MyDictionaryImportDialog.h"

#include <QPushButton>
#include <QFileDialog>
#include <QErrorMessage>
#include <QMessageBox>

#include "Application/Import/AndroidMyDictionary.h"

#include "MainWindow.h"

MyDictionaryImportDialog::MyDictionaryImportDialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::MyDictionaryImportDialog )
{
    ui->setupUi( this );
    waitingSpinner	= new VsWaitingSpinner( this );

    mw = parent;

    connect( ui->btnDatabaseBrowse, SIGNAL( clicked() ), this, SLOT( setDatabase() ) );

    QPushButton *saveButton = ui->buttonBox->button( QDialogButtonBox::Save );
    saveButton->setText( tr( "Import" ) );
    connect( saveButton, SIGNAL( clicked() ), this, SLOT( save() ) );

    // May be there is other way to translate Core Strings
    ui->buttonBox->button( QDialogButtonBox::Cancel )->setText( tr( "Cancel" ) );
}

MyDictionaryImportDialog::~MyDictionaryImportDialog()
{
    delete ui;
}

void MyDictionaryImportDialog::save()
{
	if ( dbPath.isEmpty() ) {
		QErrorMessage* errorMessage	= new QErrorMessage( this );
		errorMessage->showMessage( "You have to select a Valid DB File First !" );

		return;
	}

	QString groupName	= ui->leImport2Group->text();
	if ( groupName.isEmpty() ) {
		QErrorMessage* errorMessage	= new QErrorMessage( this );
		errorMessage->showMessage( "You have to write the name of the group to import !" );

		return;
	}

	waitingSpinner->start();
	bool importStatus	= AndroidMyDictionary::importFromDb( dbPath, groupName );
	waitingSpinner->stop();

	if ( ! importStatus ) {
		QMessageBox::critical(
			this,
			tr( "Error" ),
			tr( "Import Source Database Cannot Be Opened !" )
		);
	}
}

void MyDictionaryImportDialog::setDatabase()
{
	dbPath	= QFileDialog::getOpenFileName(
		this,
		tr( "Open Android MyDictionary Database" ),
		QDir::homePath(),
		tr( "Android MyDictionary Database (*.xls *.csv)" )
	);
	ui->leDatabase->setText( dbPath );
}
