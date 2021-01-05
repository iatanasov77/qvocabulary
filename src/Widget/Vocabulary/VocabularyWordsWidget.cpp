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
    hideColumns = {0, 2, 4};

    initModel();
    initContextMenu();

    connect( ui->chkShowTranscriptions, SIGNAL( stateChanged( int ) ), this, SLOT( showTranscriptions( int ) ) );
    connect( ui->leSearch, SIGNAL( returnPressed() ), ui->btnSearch, SIGNAL( released() ) );
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

	connect(
		pModel,
		SIGNAL( rowsInserted( const QModelIndex&, int, int ) ),
		this,
		SLOT( modelRowsInserted( const QModelIndex&, int, int ) )
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
	pModel->setHeaderData( 2, Qt::Horizontal, tr( "Transcription" ), Qt::DisplayRole );
	pModel->setHeaderData( 3, Qt::Horizontal, metaInfo->language2, Qt::DisplayRole );
}

void VocabularyWordsWidget::insertWord()
{
	pModel->insertRow( pModel->rowCount( QModelIndex() ) );
}

/**
 * NOTE: Dont call this function. It is called in VocabularyWidget::loadGroup( int groupId )
 * 		 There is functionality to remember current group into the settings.
 */
void VocabularyWordsWidget::loadGroup( int groupId )
{
	ui->leSearch->setText( "" );	// Clear Serch Field
	QString query	= QString( "WHERE group_id=%1" ).arg( groupId );
	pModel->qxFetchByQuery( query );
	//ui->stackedWidget->setCurrentWidget( ui->tableView );
	ui->stackedWidget->setCurrentWidget( ui->pageVocabulary );

	currentGroup = groupId;
}

int VocabularyWordsWidget::deleteGroup( int groupId )
{
	QString query	= QString( "WHERE group_id=%1" ).arg( QString::number( groupId ) );
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
	if (
		topLeft == bottomRight &&
		( topLeft.column() == 2 || ! hideColumns.contains( topLeft.column() ) ) // Transcription column or Hidden Column :)
	) { // Because cannot be edited(Moved to another group)
		pModel->setData( topLeft.siblingAtColumn( 4 ), QVariant( currentGroup ) );
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
		//qDebug() << "Old Group: " << pModel->data( selectedRows[i].siblingAtColumn( 4 ) ).toInt();

		//ui->tableView->model()->setData( selectedRows[i].siblingAtColumn( 4 ), QVariant( groupId ) );
		pModel->setData( selectedRows[i].siblingAtColumn( 4 ), QVariant( groupId ) );

		qDebug() << "New Group: " << pModel->data( selectedRows[i].siblingAtColumn( 4 ) ).toInt();
	}

	pModel->qxSave();
	refreshView( selectedRows[0].siblingAtColumn( 0 ), selectedRows[selectedRows.size()-1].siblingAtColumn( 4 ) );
}

void VocabularyWordsWidget::deleteWord()
{
	QList<QModelIndex> selectedRows	= ui->tableView->selectionModel()->selectedRows();

	for ( int i = 0; i < selectedRows.size(); ++i ) {
		pModel->qxDeleteById( pModel->data( selectedRows[i].siblingAtColumn( 0 ) ) );
	}
	pModel->qxSave();
	refreshView( selectedRows[0].siblingAtColumn( 0 ), selectedRows[selectedRows.size()-1].siblingAtColumn( 4 ) );
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
		groupId	= searchModel->data( searchModel->index( i, 4 ) ).toInt();
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

void VocabularyWordsWidget::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}

void VocabularyWordsWidget::modelRowsInserted( const QModelIndex & parent, int start, int end )
{
	//qDebug() << "Row Inserted: " << start;
	//ui->tableView->scrollTo( pModel->index( start, 1 ) );
	ui->tableView->scrollToBottom();
}

void VocabularyWordsWidget::showTranscriptions( int state )
{
	if ( state == Qt::Checked ) {
		ui->tableView->showColumn( 2 );
	} else {
		ui->tableView->hideColumn( 2 );
	}
}
