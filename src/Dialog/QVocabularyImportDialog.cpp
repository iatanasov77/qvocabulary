#include "QVocabularyImportDialog.h"
#include "ui_QVocabularyImportDialog.h"

#include <QPushButton>
#include <QFileDialog>
#include <QErrorMessage>
#include <QMessageBox>

#include "Application/Import/QVocabulary.h"

QVocabularyImportDialog::QVocabularyImportDialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::QVocabularyImportDialog )
{
    ui->setupUi( this );

    mw = parent;

    connect( ui->btnDatabaseBrowse, SIGNAL( clicked() ), this, SLOT( setDatabase() ) );

    QPushButton *saveButton = ui->buttonBox->button( QDialogButtonBox::Save );
    saveButton->setText( tr( "Import" ) );
    connect( saveButton, SIGNAL( clicked() ), this, SLOT( save() ) );

    // May be there is other way to translate Core Strings
    ui->buttonBox->button( QDialogButtonBox::Cancel )->setText( tr( "Cancel" ) );
}

QVocabularyImportDialog::~QVocabularyImportDialog()
{
    delete ui;
}

void QVocabularyImportDialog::save()
{
	if ( dbPath.isEmpty() ) {
		QErrorMessage* errorMessage	= new QErrorMessage( this );
		errorMessage->showMessage( "You have to select a Valid DB File First !" );

		return;
	}

	QApplication::setOverrideCursor( Qt::WaitCursor );
	bool importStatus	= QVocabulary::importFromDb( dbPath, ui->chkImportQuizes->isChecked(), ui->chkImportArchive->isChecked() );
	QApplication::restoreOverrideCursor();
	if ( ! importStatus ) {
		QMessageBox::critical(
			this,
			tr( "Error" ),
			tr( "Import Source Database Cannot Be Opened !" )
		);
	}
}

void QVocabularyImportDialog::setDatabase()
{
	dbPath	= QFileDialog::getOpenFileName(
		this,
		tr( "Open VankoSoft QVocabulary Database" ),
		QDir::homePath(),
		tr( "VankoSoft QVocabulary Database (*.db)" )
	);
	ui->leDatabase->setText( dbPath );
}
