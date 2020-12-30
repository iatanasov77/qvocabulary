#include "VocabularyWidget.h"
#include "ui_VocabularyWidget.h"

#include <QListView>
#include <QTableView>
#include <QHeaderView>
#include <QSplitter>
#include <QMenu>
#include <QSettings>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Application/VsDatabase.h"
#include "Application/VsSettings.h"
#include "Entity/VocabularyMetaInfo.h"
#include "Entity/Vocabulary.h"
#include "Entity/VocabularyGroup.h"
#include "ModelView/SideBarListViewDelegate.h"

#include "Widget/Vocabulary/VocabularyGroupsWidget.h"
#include "Widget/Vocabulary/VocabularyWordsWidget.h"

VocabularyWidget::VocabularyWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::VocabularyWidget )
{
    ui->setupUi( this );

    init();
    initModels();

    // Load Current Group by settings or first loaded
    QSettings* settings	= VsSettings::instance()->settings();
    int currentGroup	= settings->value( "currentGroup" ).toInt();
    if ( currentGroup ) {
    	loadGroup( currentGroup );
    } else {
    	loadGroup( wdgGroups->currentGroup() );
    }
}

VocabularyWidget::~VocabularyWidget()
{
    delete ui;
}

void VocabularyWidget::init()
{
	QSplitter* horizSplitter 	= new QSplitter( Qt::Horizontal );
	wdgGroups					= new VocabularyGroupsWidget( this );
	wdgWords					= new VocabularyWordsWidget( this );

	wdgGroups->setMaximumWidth( 400 );
	wdgGroups->adjustSize();

	horizSplitter->insertWidget( 0, wdgGroups );
	horizSplitter->insertWidget( 1, wdgWords );
	ui->horizontalLayout->addWidget( horizSplitter );
}

void VocabularyWidget::initModels()
{
	// Init VocabularyMetaInfo
	VocabularyMetaInfoPtr metaInfo	= VsDatabase::instance()->metaInfo();
	if ( metaInfo ) {
		ui->databaseName->setText( metaInfo->name );
	}

	wdgWords->setViewHeader( metaInfo );
}

void VocabularyWidget::setCurrentGroupName( QString groupName )
{
	ui->groupName->setText( groupName );
}

void VocabularyWidget::insertWord()
{
	wdgWords->insertWord();
}

void VocabularyWidget::loadGroup( int groupId )
{
	QSettings* settings	= VsSettings::instance()->settings();
	currentGroup		= groupId;
	wdgWords->loadGroup( groupId );

	settings->setValue( "currentGroup", currentGroup );
	settings->sync();	// Sync ini file
}

void VocabularyWidget::deleteGroup( int groupId )
{
	currentGroup	= wdgWords->deleteGroup( groupId );
}

void VocabularyWidget::loadGroup( const QModelIndex &index )
{
    int groupId			= index.siblingAtColumn( 0 ).data().toInt();
    QString groupName	= index.siblingAtColumn( 1 ).data().toString();

	setCurrentGroupName( groupName );
    loadGroup( groupId );
}

void VocabularyWidget::refreshWidgets()
{
	init();
}