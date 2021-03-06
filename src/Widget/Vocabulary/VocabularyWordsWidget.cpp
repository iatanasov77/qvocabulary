#include "VocabularyWordsWidget.h"
#include "ui_VocabularyWordsWidget.h"

#include <QApplication>
#include <QTableView>
#include <QMenu>
#include <QTextToSpeech>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Application/VsDatabase.h"
#include "Application/VsSpeaker.h"
#include "Entity/VocabularyMetaInfo.h"
#include "Entity/Vocabulary.h"
#include "Entity/VocabularyGroup.h"

#include "VocabularyWidget.h"
#include "ModelView/Helper.h"
#include "ModelView/VocabularyTableView.h"
#include "ModelView/VocabularyTableViewDelegate.h"
#include "ModelView/VocabularyWordsModel.h"
#include "Dialog/AddDescriptionDialog.h"

VocabularyWordsWidget::VocabularyWordsWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::VocabularyWordsWidget )
{
    ui->setupUi( this );
    initView();

    //QVBoxLayout *tableLayout	= new QVBoxLayout( ui->tableView );

    currentGroup 	= 1;
    hideColumns 	= {0, 2, 4, 5};

    initModel();
    adjustRowSelection();
    initContextMenu();
    initTextToSpeech();

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
	pModel	= new VocabularyWordsModel();

	VocabularyTableViewDelegate* itemDelegate	= new VocabularyTableViewDelegate( ui->tableView );
	ui->tableView->setItemDelegateForColumn( 2, itemDelegate );
	//ui->tableView->setItemDelegate( itemDelegate );

	ui->tableView->setModel( pModel );
	//ui->tableView->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
	ui->tableView->horizontalHeader()->resizeSections( QHeaderView::ResizeToContents );
	ui->tableView->horizontalHeader()->setStretchLastSection( true );
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

	connect(
		pModel,
		SIGNAL( modelUpdated() ),
		this,
		SLOT( updateView() )
	);

	connect(
		itemDelegate,
		SIGNAL( buttonClicked( QModelIndex ) ),
		this,
		SLOT( sayWord( QModelIndex ) )
	);
}

void VocabularyWordsWidget::initContextMenu()
{
	ui->tableView->setContextMenuPolicy( Qt::CustomContextMenu );
	connect(
		ui->tableView,
		SIGNAL( customContextMenuRequested( QPoint ) ),
		this,
		SLOT( displayContextMenu( QPoint ) )
	);
}

void VocabularyWordsWidget::initTextToSpeech()
{
	speeker = new VsSpeaker( this );
}

void VocabularyWordsWidget::setViewHeader( VocabularyMetaInfoPtr metaInfo )
{
	QStringList headTitles	= viewHeaders( metaInfo );

	pModel->setHeaderData( 1, Qt::Horizontal, headTitles.at( 0 ), Qt::DisplayRole );
	pModel->setHeaderData( 2, Qt::Horizontal, headTitles.at( 2 ), Qt::DisplayRole );
	pModel->setHeaderData( 3, Qt::Horizontal, headTitles.at( 1 ), Qt::DisplayRole );
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

void VocabularyWordsWidget::updateView()
{
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
	QMenu* menuMoveToGroup	= menu->addMenu( tr( "Move to Group" ) );

	QAction* actDeleteWord		= new QAction( this );
	actDeleteWord->setText( tr( "Delete Word" ) );
	actDeleteWord->setIcon( QIcon( ":/Resources/icons/amarok_cart_remove.svg" ) );
	connect( actDeleteWord, &QAction::triggered, this, &VocabularyWordsWidget::deleteWord );

	menu->addSeparator();
	menu->addAction( actDeleteWord );

	QAction* actAddDescription	= new QAction( this );
	actAddDescription->setText( tr( "Add Word Description" ) );
	actAddDescription->setIcon( QIcon( ":/Resources/icons/mail-message-new.svg" ) );
	connect( actAddDescription, &QAction::triggered, this, &VocabularyWordsWidget::addWordDescription );

	menu->addSeparator();
	menu->addAction( actAddDescription );

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
	QStringList headTitles			= viewHeaders( metaInfo );

	QStringList headerLabels;
	headerLabels << "\t" + headTitles.at( 0 );
	if ( ui->chkShowTranscriptions->isChecked() ) {
		headerLabels << headTitles.at( 2 );
	}
	headerLabels << headTitles.at( 1 );
	ui->treeWidget->setHeaderLabels( headerLabels );

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
		if ( ui->chkShowTranscriptions->isChecked() ) {
			QFont font;
			font.setBold( true );
			childItem->setText( 1, searchModel->data( searchModel->index( i, 2 ) ).toString() );
			childItem->setFont( 1, font );

			childItem->setText( 2, searchModel->data( searchModel->index( i, 3 ) ).toString() );
		} else {
			childItem->setText( 1, searchModel->data( searchModel->index( i, 3 ) ).toString() );
		}

		groups[groupId]->addChild( childItem );
	}

	// Resize Columns
	ui->treeWidget->setColumnWidth( 0 , 200 );

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
	Q_UNUSED( parent );
	Q_UNUSED( start );
	Q_UNUSED( end );

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

void VocabularyWordsWidget::adjustRowSelection()
{
	ui->tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
	//ui->tableView->setSelectionMode( QAbstractItemView::SingleSelection );	// SingleSelection because easyer
	ui->tableView->setSelectionMode( QAbstractItemView::ExtendedSelection );
}

void VocabularyWordsWidget::sayWord( const QModelIndex &index )
{
    QString word	= index.siblingAtColumn( 1 ).data().toString();

    speeker->tts()->say( word );
}

void VocabularyWordsWidget::updateSpeaker()
{
	speeker->updateTts();
}

QStringList VocabularyWordsWidget::viewHeaders( VocabularyMetaInfoPtr metaInfo )
{
	QStringList headTitles;
	headTitles
		<< qApp->translate( "Vocabulary", metaInfo->language1.toStdString().c_str() )
		<< qApp->translate( "Vocabulary", metaInfo->language2.toStdString().c_str() )
		<< qApp->translate( "Vocabulary", "Transcription" );

	return headTitles;
}

QMap<QString, QVariant> VocabularyWordsWidget::getState()
{
	QMap<QString, QVariant> widgetState;

	widgetState["showTranscriptions"]	= QVariant( ui->chkShowTranscriptions->isChecked() );
	//widgetState["showTranscriptions"]	= QVariant( true );

	return widgetState;
}

void VocabularyWordsWidget::setState( QMap<QString, QVariant> state )
{
	if ( state.contains( "showTranscriptions" ) ) {
		ui->chkShowTranscriptions->setChecked( state["showTranscriptions"].toBool() );
	}
}

void VocabularyWordsWidget::addWordDescription()
{
	QList<QModelIndex> selectedRows	= ui->tableView->selectionModel()->selectedRows();
	if ( selectedRows.size() > 1 ) {
		// Show Error Message
		return;
	}

	QMap<QString, QString> firstWord;
	QList<QMap<QString, QString>> selectedWords;
	firstWord["word"]			= pModel->data( selectedRows[0].siblingAtColumn( 1 ) ).toString();
	firstWord["description"]	= pModel->data( selectedRows[0].siblingAtColumn( 5 ) ).toString();
	selectedWords << firstWord;

	AddDescriptionDialog *dlg	= new AddDescriptionDialog( selectedWords, this );
	dlg->setModal( true );
	dlg->show();
	if ( dlg->exec() == QDialog::Accepted ) {
		pModel->setData( selectedRows[0].siblingAtColumn( 5 ), QVariant( dlg->getDescription() ) );
		pModel->qxSave();
	}
}

void VocabularyWordsWidget::initView()
{
	ui->verticalLayout_3->removeWidget( ui->tableView );
	ui->tableView	= new VocabularyTableView( ui->pageVocabulary );
	ui->verticalLayout_3->addWidget( ui->tableView );
}
