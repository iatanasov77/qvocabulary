#include "RenameVocabularyGroupDialog.h"
#include "ui_NewVocabularyGroupDialog.h"

#include <QPushButton>

RenameVocabularyGroupDialog::RenameVocabularyGroupDialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::NewVocabularyGroupDialog )
{
    ui->setupUi( this );
}

RenameVocabularyGroupDialog::RenameVocabularyGroupDialog( QString oldGroupName, QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::NewVocabularyGroupDialog )
{
    ui->setupUi( this );

    this->setWindowTitle( tr( "Rename Vocabulary Group" ) );
    ui->leName->setText( oldGroupName );

    QPushButton *renameButton = ui->buttonBox->button( QDialogButtonBox::Save );
    renameButton->setText( tr( "Rename" ) );

    // May be there is other way to translate Core Strings
    ui->buttonBox->button( QDialogButtonBox::Cancel )->setText( tr( "Cancel" ) );
}

RenameVocabularyGroupDialog::~RenameVocabularyGroupDialog()
{
    delete ui;
}

QString RenameVocabularyGroupDialog::newName()
{
	return ui->leName->text();
}
