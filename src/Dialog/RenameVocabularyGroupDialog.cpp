#include "RenameVocabularyGroupDialog.h"
#include "ui_NewVocabularyGroupDialog.h"

#include <QPushButton>

RenameVocabularyGroupDialog::RenameVocabularyGroupDialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::NewVocabularyGroupDialog )
{
    ui->setupUi( this );

    this->setWindowTitle( "Rename Vocabulary Group" );

    QPushButton *renameButton = ui->buttonBox->button( QDialogButtonBox::Save );
    renameButton->setText( "Rename" );
}

RenameVocabularyGroupDialog::~RenameVocabularyGroupDialog()
{
    delete ui;
}

QString RenameVocabularyGroupDialog::newName()
{
	return ui->leName->text();
}
