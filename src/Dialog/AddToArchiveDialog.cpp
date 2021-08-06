#include "AddToArchiveDialog.h"
#include "ui_AddToArchiveDialog.h"

#include <QPushButton>
#include <QFileDialog>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyMetaInfo.h"
#include "Entity/VocabularyGroup.h"

#include "Application/VsApplication.h"
#include "Application/VsDatabase.h"

#include "MainWindow.h"

AddToArchiveDialog::AddToArchiveDialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::AddToArchiveDialog )
{
    ui->setupUi( this );

    mw = parent;

    connect( ui->browseForDb, SIGNAL( clicked() ), this, SLOT( setDatabase() ) );

    QPushButton *saveButton = ui->buttonBox->button( QDialogButtonBox::Save );
    saveButton->setText( tr( "Add To Archive" ) );
    connect( saveButton, SIGNAL( clicked() ), this, SLOT( addToArchive() ) );
}

AddToArchiveDialog::~AddToArchiveDialog()
{
    delete ui;
}

void AddToArchiveDialog::setDatabase()
{
	QDir userAppDataPath	= VsApplication::dataPath();
	QString dbPathHome		= userAppDataPath.absoluteFilePath( "untitled.db" );

	dbPath 					= QFileDialog::getSaveFileName(
								this,
								tr( "Open Database" ),
								dbPathHome,
								tr( "*.db" )
							);

	ui->leDbPath->setText( dbPath );
}

void AddToArchiveDialog::addToArchive()
{
	if ( dbPath.isEmpty() ) {

		return;
	}

	//_addToArchive();
}

void AddToArchiveDialog::_addToArchive()
{
	QSqlError daoError;

//	VsDatabase::instance()->create( dbPath, ui->leDefaultGroupName->text() );
//
//	// Insert Default VocabularyGroup
//	VocabularyGroupPtr vocabularyGroup;
//	vocabularyGroup.reset( new VocabularyGroup() );
//	vocabularyGroup->name	= ui->leDefaultGroupName->text();
//	daoError				= qx::dao::insert( vocabularyGroup );

	// Load DB
	//qobject_cast<MainWindow*>( mw )->loadDb( dbPath );
}
