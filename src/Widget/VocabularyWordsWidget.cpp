#include "VocabularyWordsWidget.h"
#include "ui_VocabularyWordsWidget.h"

#include <QTableView>
#include <QMenu>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Application/VsDatabase.h"
#include "Entity/VocabularyMetaInfo.h"
#include "Entity/Vocabulary.h"
#include "Entity/VocabularyGroup.h"

#include "VocabularyWidget.h"
#include "ModelView/Helper.h"

VocabularyWordsWidget::VocabularyWordsWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::VocabularyWordsWidget )
{
    ui->setupUi( this );

    currentGroup = 1;
    hideColumns = {0, 3};

    initModel();
    initContextMenu();

    loadGroup( currentGroup );

    connect( ui->btnSearch, SIGNAL( released() ), this, SLOT( search() ) );
}

VocabularyWordsWidget::~VocabularyWordsWidget()
{
    delete ui;
}

void VocabularyWordsWidget::initModel()
{
	pModel	= new qx::QxModel<Vocabulary>();

	ui->tableView->setModel( pModel );
	ui->tableView->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
	for( int i = 0; i < hideColumns.size(); i++ ) {
		ui->tableView->hideColumn( hideColumns[i] );
	}

	connect(
		pModel,
		SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ),
		this,
		SLOT( onDataChanged( const QModelIndex&, const QModelIndex& ) )
	);
}

void VocabularyWordsWidget::initContextMenu()
{
	ui->tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
	ui->tableView->setSelectionMode( QAbstractItemView::ExtendedSelection );
	ui->tableView->setContextMenuPolicy( Qt::CustomContextMenu );
	connect(
		ui->tableView,
		SIGNAL( customContextMenuRequested( QPoint ) ),
		this,
		SLOT( displayContextMenu( QPoint ) )
	);
}

void VocabularyWordsWidget::setViewHeader( VocabularyMetaInfoPtr metaInfo )
{
	pModel->setHeaderData( 1, Qt::Horizontal, metaInfo->language1, Qt::DisplayRole );
	pModel->setHeaderData( 2, Qt::Horizontal, metaInfo->language2, Qt::DisplayRole );
}

void VocabularyWordsWidget::insertWord()
{
	pModel->insertRow( pModel->rowCount( QModelIndex() ) );
}

void VocabularyWordsWidget::loadGroup( int groupId )
{
	QString query	= QString( "WHERE group_id=%1" ).arg( groupId );
	pModel->qxFetchByQuery( query );
	//ui->stackedWidget->setCurrentWidget( ui->tableView );
	ui->stackedWidget->setCurrentWidget( ui->pageVocabulary );

	currentGroup = groupId;
}

int VocabularyWordsWidget::deleteGroup( int groupId )
{
	QString query	= QString( "WHERE group_id=%1" ).arg( groupId );
	pModel->qxDeleteByQuery( query );

	if ( currentGroup == groupId ) {
		currentGroup = groupId - 1;
		loadGroup( currentGroup );
	}

	return currentGroup;
}

void VocabularyWordsWidget::refreshView( QModelIndex topLeft, QModelIndex bottomRight )
{
	emit pModel->dataChanged( topLeft, bottomRight );
	initModel();
	loadGroup( currentGroup );
}

void VocabularyWordsWidget::onDataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
	if ( topLeft == bottomRight && ! hideColumns.contains( topLeft.column() ) ) { // Because cannot be edited(Moved to another group)
		pModel->setData( topLeft.siblingAtColumn( 3 ), QVariant( currentGroup ) );
		pModel->qxSave();
	}
}

void VocabularyWordsWidget::displayContextMenu( QPoint pos )
{
	QMenu* menu				= new QMenu( this );
	QMenu* menuMoveToGroup	= menu->addMenu( "Move to Group" );

	QAction* actDeleteWord		= new QAction( this );
	actDeleteWord->setText( tr( "Delete Word" ) );
	actDeleteWord->setIcon( QIcon( ":/Resources/icons/amarok_cart_remove.svg" ) );
	connect( actDeleteWord, &QAction::triggered, this, &VocabularyWordsWidget::deleteWord );

	menu->addSeparator();
	menu->addAction( actDeleteWord );

	QAction* actMove;
	QMap<int, QString> groups	= Helper::getAllGroups();
	QMapIterator<int, QString> i(groups);
	while ( i.hasNext() ) {
	    i.next();

	    actMove	= new QAction( i.value(), this );
	    actMove->setData( QVariant( i.key() ) );
	    connect( actMove, &QAction::triggered, this, &VocabularyWordsWidget::moveToGroup );

	    menuMoveToGroup->addAction( actMove );
	}

	menu->popup( ui->tableView->viewport()->mapToGlobal( pos ) );
}

void VocabularyWordsWidget::moveToGroup()
{
	QAction *action 				= qobject_cast<QAction *>( sender() );
	int groupId						= action->data().toInt();

	QList<QModelIndex> selectedRows	= ui->tableView->selectionModel()->selectedRows();
	for ( int i = 0; i < selectedRows.size(); ++i ) {
		qDebug() << "Old Group: " << pModel->data( selectedRows[i].siblingAtColumn( 3 ) ).toInt();

		//ui->tableView->model()->setData( selectedRows[i].siblingAtColumn( 3 ), QVariant( groupId ) );
		pModel->setData( selectedRows[i].siblingAtColumn( 3 ), QVariant( groupId ) );

		qDebug() << "New Group: " << pModel->data( selectedRows[i].siblingAtColumn( 3 ) ).toInt();
	}

	pModel->qxSave();
	refreshView( selectedRows[0].siblingAtColumn( 0 ), selectedRows[selectedRows.size()-1].siblingAtColumn( 3 ) );
}

void VocabularyWordsWidget::deleteWord()
{
	QList<QModelIndex> selectedRows	= ui->tableView->selectionModel()->selectedRows();

	for ( int i = 0; i < selectedRows.size(); ++i ) {
		pModel->qxDeleteById( pModel->data( selectedRows[i].siblingAtColumn( 0 ) ) );
	}
	pModel->qxSave();
	refreshView( selectedRows[0].siblingAtColumn( 0 ), selectedRows[selectedRows.size()-1].siblingAtColumn( 3 ) );
}

void VocabularyWordsWidget::search()
{
	qx::QxModel<Vocabulary>* searchModel	= new qx::QxModel<Vocabulary>();
	QString searchWord						= ui->leSearch->text();
	QString query							= QString( "WHERE language_1 LIKE '%%1%' OR language_2 LIKE '%%1%'" ).arg( searchWord );
	searchModel->qxFetchByQuery( query );

	displaySearchResults( searchModel );
}

void VocabularyWordsWidget::displaySearchResults( qx::QxModel<Vocabulary>* searchModel )
{
	int groupId;
	QString groupName;
	QMap<int, QTreeWidgetItem*> groups;
	qx::QxModel<VocabularyGroup>* groupModel	= new qx::QxModel<VocabularyGroup>();

	ui->treeWidget->clear();

	QTreeWidgetItem* childItem;
	VocabularyMetaInfoPtr metaInfo	= VsDatabase::instance()->metaInfo();
	ui->treeWidget->setHeaderLabels( QStringList() << "\t" + metaInfo->language1 << metaInfo->language2 );
	for ( int i = 0; i < searchModel->rowCount(); ++i ) {
		groupId	= searchModel->data( searchModel->index( i, 3 ) ).toInt();
		if ( ! groups.contains( groupId ) ) {
			VocabularyGroupPtr group;
			group.reset( new VocabularyGroup() );
			group->id 	= groupId;
			groupModel->qxFetchById( groupId );
			groupName	= groupModel->data( groupModel->index( 0, 1 ) ).toString();

			groups[groupId]	= new QTreeWidgetItem( ui->treeWidget );
			groups[groupId]->setText( 0, groupName );
			ui->treeWidget->expandItem( groups[groupId] );
		}

		childItem = new QTreeWidgetItem();
		childItem->setText( 0, searchModel->data( searchModel->index( i, 1 ) ).toString() );
		childItem->setText( 1, searchModel->data( searchModel->index( i, 2 ) ).toString() );
		groups[groupId]->addChild( childItem );
	}

	ui->stackedWidget->setCurrentWidget( ui->pageSearch );
}
