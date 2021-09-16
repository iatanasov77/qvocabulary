#include "ImportArchiveDialog.h"
#include "ui_ImportArchiveDialog.h"

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

ImportArchiveDialog::ImportArchiveDialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::ImportArchiveDialog )
{
    ui->setupUi( this );

    connect( ui->browseForDb, SIGNAL( clicked() ), this, SLOT( setDatabase() ) );

    QPushButton *saveButton = ui->buttonBox->button( QDialogButtonBox::Save );
    saveButton->setText( tr( "Import Archive" ) );
    connect( saveButton, SIGNAL( clicked() ), this, SLOT( importArchive() ) );
}

ImportArchiveDialog::~ImportArchiveDialog()
{
    delete ui;
    importedGroups.clear();
}

void ImportArchiveDialog::setDatabase()
{
	dbPath	= QFileDialog::getOpenFileName( this, tr( "Open Database" ), QDir::homePath(), tr( "Db Files (*.db)" ) );
	ui->leDbPath->setText( dbPath );
}

void ImportArchiveDialog::importArchive()
{
	if ( dbPath.isEmpty() ) {
		QErrorMessage* errorMessage	= new QErrorMessage( this );
		errorMessage->showMessage( "You have to select a Valid DB File First !" );

		return;
	}

	QApplication::setOverrideCursor( Qt::WaitCursor );
	QSqlDatabase db	= QSqlDatabase::addDatabase( "QSQLITE", "import_source" );
	db.setDatabaseName( dbPath );
	db.setHostName( "localhost" );
	db.setUserName( "root" );
	db.setPassword( "root" );

	if ( db.open() ) {
		_importArchiveGroups( db );
		_importArchiveWords( db );
		db.close();
	} else {
		qDebug() << "Import Source Database Cannot Be Opened!";
	}
	QApplication::restoreOverrideCursor();
}

void ImportArchiveDialog::_importArchiveGroups( QSqlDatabase db )
{
	ArchiveGroupPtr archg;
	QString groupName;
	qx::QxModel<ArchiveGroup> *pModelArchiveGroup	= new qx::QxModel<ArchiveGroup>();
	int row											= -1;
	int currentGroup								= -1;

	QSqlQuery query( "SELECT * FROM ArchiveGroup", db );
	while ( query.next() ) {
		currentGroup	= query.value( "id" ).toInt();
		groupName		= query.value( "name" ).toString();

		// Some Fix for Empty Group Names but I dont know if this is the right in Import Functionality
		if ( groupName.length() ) {
			row	= pModelArchiveGroup->rowCount( QModelIndex() );
			pModelArchiveGroup->insertRow( row );
			pModelArchiveGroup->setData( pModelArchiveGroup->index( row, 1 ), groupName );
			pModelArchiveGroup->qxSave();

			importedGroups[currentGroup]	= pModelArchiveGroup->data( pModelArchiveGroup->index( row, 0 ) ).toInt();
		}
	}
}

void ImportArchiveDialog::_importArchiveWords( QSqlDatabase db )
{
	QSqlError daoError;
	ArchiveWordPtr aw;

	QSqlQuery query( "SELECT * FROM ArchiveWord", db );
	while ( query.next() ) {
		aw					= ArchiveWordPtr( new ArchiveWord() );
		aw->group_id		= importedGroups[query.value( "group_id" ).toInt()];
		aw->language_1		= query.value( "language_1" ).toString();

		aw->transcription	= query.value( "transcription" ).toString();
		aw->language_2		= query.value( "language_2" ).toString();
		aw->description		= query.value( "description" ).toString();

		daoError			= qx::dao::save( aw );
	}
}
