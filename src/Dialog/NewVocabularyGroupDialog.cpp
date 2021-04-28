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
	mw 		= qobject_cast<MainWindow*>( parent );

    ui->setupUi( this );

    QPushButton *saveButton = ui->buttonBox->button( QDialogButtonBox::Save );
    connect( saveButton, SIGNAL( clicked() ), this, SLOT( save() ) );
}

NewVocabularyGroupDialog::~NewVocabularyGroupDialog()
{
    delete ui;
}

void NewVocabularyGroupDialog::reloadWidgets()
{
	mw->initWidgets();
}

void NewVocabularyGroupDialog::save()
{
	qx::QxModel<VocabularyGroup> *pModelVocabularyGroup	= new qx::QxModel<VocabularyGroup>();

	int row	= pModelVocabularyGroup->rowCount( QModelIndex() );
	pModelVocabularyGroup->insertRow( row );
	pModelVocabularyGroup->setData( pModelVocabularyGroup->index( row, 1 ), ui->leName->text() );
	pModelVocabularyGroup->qxSave();
	reloadWidgets();

	// Set Current group to the new created group
	int newGroupId	= pModelVocabularyGroup->data( pModelVocabularyGroup->index( row, 0 ) ).toInt();
	mw->setCurrentGroup( newGroupId );
	reloadWidgets();
}
