#include "AddToArchiveDialog.h"
#include "ui_AddToArchiveDialog.h"

#include <QPushButton>
#include <QFileDialog>
#include <QErrorMessage>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/ArchiveGroup.h"
#include "Entity/ArchiveWord.h"

#include "Application/VsApplication.h"
#include "Application/VsDatabase.h"

#include "MainWindow.h"

AddToArchiveDialog::AddToArchiveDialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::AddToArchiveDialog )
{
    ui->setupUi( this );
    initGroupsCombo();

    mw = parent;

    connect( ui->browseForDb, SIGNAL( clicked() ), this, SLOT( setDatabase() ) );

    QPushButton *saveButton = ui->buttonBox->button( QDialogButtonBox::Save );
    saveButton->setText( tr( "Add To Archive" ) );
    connect( saveButton, SIGNAL( clicked() ), this, SLOT( addToArchive() ) );

    connect( ui->cbArchiveGroup, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onGroupsComboChanged( int ) ) );
}

AddToArchiveDialog::~AddToArchiveDialog()
{
    delete ui;
}

void AddToArchiveDialog::setDatabase()
{
	dbPath	= QFileDialog::getOpenFileName( this, tr( "Open Database" ), QDir::homePath(), tr( "Db Files (*.db)" ) );
	ui->leDbPath->setText( dbPath );
}

void AddToArchiveDialog::addToArchive()
{
	if ( dbPath.isEmpty() ) {
		QErrorMessage* errorMessage	= new QErrorMessage( this );
		errorMessage->showMessage( "You have to select a Valid DB File First !" );

		return;
	}

	QSqlDatabase db	= QSqlDatabase::addDatabase( "QSQLITE", "import_source" );
	db.setDatabaseName( dbPath );
	db.setHostName( "localhost" );
	db.setUserName( "root" );
	db.setPassword( "root" );

	int groupId			= ui->cbArchiveGroup->currentData().toInt();
	QString groupName	= ui->leArchiveGroupName->text();
	if ( groupId <= 0 && groupName.length() ) {
		groupId	= _createArchiveGroup( groupName );
	}
	if ( db.open() && groupId ) {
		_addToArchive( db, groupId );
		db.close();
	} else {
		qDebug() << "Import Source Database Cannot Be Opened!";
	}
}

int AddToArchiveDialog::_createArchiveGroup( QString groupName )
{
	qx::QxModel<ArchiveGroup> *pModelArchiveGroup	= new qx::QxModel<ArchiveGroup>();
	int row											= pModelArchiveGroup->rowCount( QModelIndex() );

	pModelArchiveGroup->insertRow( row );
	pModelArchiveGroup->setData( pModelArchiveGroup->index( row, 1 ), groupName );
	pModelArchiveGroup->qxSave();

	return pModelArchiveGroup->data( pModelArchiveGroup->index( row, 0 ) ).toInt();
}

void AddToArchiveDialog::_addToArchive( QSqlDatabase db, int archiveGroupId )
{
	QSqlError daoError;
	ArchiveWordPtr aw;

	QSqlQuery query( "SELECT * FROM Vocabulary", db );
	while ( query.next() ) {
		aw					= getArchiveWord( query.value( "language_1" ).toString(), archiveGroupId );

		aw->transcription	= query.value( "transcription" ).toString();
		aw->language_2		= query.value( "language_2" ).toString();
		aw->description		= query.value( "description" ).toString();

		daoError			= qx::dao::save( aw );
	}
}

void AddToArchiveDialog::initGroupsCombo()
{
	ui->cbArchiveGroup->addItem( "-- Select Existing Group --", QVariant( -1 ) );

	QSqlQuery archiveGroupQuery( "SELECT * FROM ArchiveGroup",  qx::QxSqlDatabase::getDatabase() );
	while ( archiveGroupQuery.next() ) {
		ui->cbArchiveGroup->addItem( archiveGroupQuery.value( "name" ).toString(), archiveGroupQuery.value( "id" ) );
	}
}

void AddToArchiveDialog::onGroupsComboChanged( int index )
{
	int groupId	= ui->cbArchiveGroup->itemData( index ).toInt();
	if ( groupId > 0 ) {
		ui->frmNewGroup->hide();
	} else {
		ui->frmNewGroup->show();
	}
}

ArchiveWordPtr AddToArchiveDialog::getArchiveWord( QString word, int archiveGroupId )
{
	ArchiveWordPtr aw;
	QString checkQuery	= QString( "SELECT * FROM ArchiveWord WHERE language_1='%1' AND group_id=%2" )
							.arg( word )
							.arg( archiveGroupId );
	QSqlQuery query( checkQuery, qx::QxSqlDatabase::getDatabase() );

	aw					= ArchiveWordPtr( new ArchiveWord() );
	if ( query.next() ) {
		aw->id	= query.value( "id" ).toInt();
	}
	aw->group_id		= archiveGroupId;
	aw->language_1		= word;

	return aw;
}
