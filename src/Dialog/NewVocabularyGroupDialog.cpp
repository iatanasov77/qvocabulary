#include "NewVocabularyGroupDialog.h"
#include "ui_NewVocabularyGroupDialog.h"

#include <QPushButton>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "MainWindow.h"
#include "Entity/VocabularyGroup.h"

NewVocabularyGroupDialog::NewVocabularyGroupDialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::NewVocabularyGroupDialog )
{
    ui->setupUi( this );

    QPushButton *saveButton = ui->buttonBox->button( QDialogButtonBox::Save );
    saveButton->setText( tr( "Save" ) );
    connect( saveButton, SIGNAL( clicked() ), this, SLOT( save() ) );

    // May be there is other way to translate Core Strings
    ui->buttonBox->button( QDialogButtonBox::Cancel )->setText( tr( "Cancel" ) );
}

NewVocabularyGroupDialog::~NewVocabularyGroupDialog()
{
    delete ui;
}

void NewVocabularyGroupDialog::save()
{
	qx::QxModel<VocabularyGroup> *pModelVocabularyGroup	= new qx::QxModel<VocabularyGroup>();

	int row	= pModelVocabularyGroup->rowCount( QModelIndex() );
	pModelVocabularyGroup->insertRow( row );
	pModelVocabularyGroup->setData( pModelVocabularyGroup->index( row, 1 ), ui->leName->text() );
	pModelVocabularyGroup->qxSave();
}
