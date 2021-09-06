#include "NewDatabaseDialog.h"
#include "ui_NewDatabaseDialog.h"

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

NewDatabaseDialog::NewDatabaseDialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::NewDatabaseDialog )
{
    ui->setupUi( this );

    mw = parent;

    QPushButton *saveButton = ui->buttonBox->button( QDialogButtonBox::Save );
    saveButton->setText( tr( "Save" ) );
    connect( saveButton, SIGNAL( clicked() ), this, SLOT( save() ) );

    // May be there is other way to translate Core Strings
    ui->buttonBox->button( QDialogButtonBox::Cancel )->setText( tr( "Cancel" ) );
}

NewDatabaseDialog::~NewDatabaseDialog()
{
    delete ui;
}

void NewDatabaseDialog::save()
{
	QSqlError daoError;
	QDir userAppDataPath	= VsApplication::dataPath();
	QString dbPathHome		= userAppDataPath.absoluteFilePath( "untitled.db" );

	dbPath 					= QFileDialog::getSaveFileName(
								this,
								tr( "Create Database" ),
								dbPathHome,
								tr( "*.db" )
							);

	if ( ! dbPath.isEmpty() ) {
		VsDatabase::instance()->create( dbPath, ui->leDefaultGroupName->text() );

		// Insert VocabularyMetaInfo
		VocabularyMetaInfoPtr metaInfo;
		metaInfo.reset( new VocabularyMetaInfo() );
		metaInfo->name			= ui->leName->text();
		metaInfo->language1		= ui->leLanguage_1->text();
		metaInfo->language2		= ui->leLanguage_2->text();
		metaInfo->dbVersion		= VsApplication::DB_VERSION;
		daoError				= qx::dao::insert( metaInfo );

		// Insert Default VocabularyGroup
		VocabularyGroupPtr vocabularyGroup;
		vocabularyGroup.reset( new VocabularyGroup() );
		vocabularyGroup->name	= ui->leDefaultGroupName->text();
		daoError				= qx::dao::insert( vocabularyGroup );

		// Load DB
		//qobject_cast<MainWindow*>( mw )->loadDb( dbPath );
	}
}

QString NewDatabaseDialog::database()
{
	return dbPath;
}
