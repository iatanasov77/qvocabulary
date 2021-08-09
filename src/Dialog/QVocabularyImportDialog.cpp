#include "QVocabularyImportDialog.h"
#include "ui_QVocabularyImportDialog.h"

#include <QPushButton>
#include <QFileDialog>
#include <QErrorMessage>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyGroup.h"
#include "Entity/Vocabulary.h"

#include "Application/VsApplication.h"
#include "Application/VsDatabase.h"
#include "Application/Import/QVocabulary.h"

#include "MainWindow.h"

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

	QVocabulary::importFromDb( dbPath, ui->chkImportQuizes->isChecked(), ui->chkImportArchive->isChecked() );
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
