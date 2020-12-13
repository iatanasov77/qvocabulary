#include "VocabularyWidget.h"
#include "ui_VocabularyWidget.h"

#include <QListView>
#include <QTableView>
#include <QHeaderView>
#include <QSplitter>
#include <QMenu>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyMetaInfo.h"
#include "Entity/Vocabulary.h"
#include "Entity/VocabularyGroup.h"
#include "ModelView/SideBarListViewDelegate.h"

#include "Widget/VocabularyGroupsWidget.h"
#include "Widget/VocabularyWordsWidget.h"

VocabularyWidget::VocabularyWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::VocabularyWidget )
{
    ui->setupUi( this );

    init();
    initModels();
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
	VocabularyMetaInfoPtr metaInfo;
	metaInfo.reset( new VocabularyMetaInfo() );
	metaInfo->id = 1;
	QSqlError daoError	= qx::dao::fetch_by_id( metaInfo );
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
	currentGroup	= groupId;
	wdgWords->loadGroup( groupId );
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
    wdgWords->loadGroup( groupId );
}
