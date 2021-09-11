#include "VocabularyWordsWidget.h"
#include "ui_VocabularyWordsWidget.h"

#include <QApplication>
#include <QTableView>
#include <QMenu>
#include <QTextToSpeech>
#include <QMessageBox>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "GlobalTypes.h"
#include "Application/VsSettings.h"
#include "Application/VsDatabase.h"
#include "Application/VsSpeaker.h"
#include "Entity/VocabularyMetaInfo.h"
#include "Entity/Vocabulary.h"
#include "Entity/VocabularyGroup.h"
#include "Entity/ArchiveWord.h"
#include "Entity/ArchiveGroup.h"

#include "Widget/Vocabulary/VocabularyWidget.h"
#include "Model/Helper.h"
#include "View/VocabularyTableView.h"
#include "View/ViewDelegate/Vocabulary/VocabularyTranscriptionsDelegate.h"
#include "View/ViewDelegate/Vocabulary/VocabularyTranslationsDelegate.h"
#include "View/ViewDelegate/Vocabulary/VocabularySynonymsDelegate.h"
#include "Dialog/AddDescriptionDialog.h"
#include "Dialog/SynonymsDialog.h"
#include "Dialog/VocabularyTranslationsTypesDialog.h"

VocabularyWordsWidget::VocabularyWordsWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::VocabularyWordsWidget )
{
    ui->setupUi( this );
    initView();
    wdgVocabulary	= parent;

    //QVBoxLayout *tableLayout	= new QVBoxLayout( ui->tableView );

    currentGroup 	= 1;
    hideColumns 	= {0, 2, 4, 5};

    adjustRowSelection();
    initModel();
    initContextMenu();
    initTextToSpeech();

    for( int i = 0; i < hideColumns.size(); i++ ) {
		ui->tableView->hideColumn( hideColumns[i] );
	}

    int displayTranscriptionsState	= VsSettings::instance()->value( "displayTranscriptions", "Vocabulary" ).toInt();
    if ( displayTranscriptionsState && displayTranscriptionsState == Qt::Checked ) {
    	ui->chkShowTranscriptions->setCheckState( Qt::Checked );
    	showTranscriptions( displayTranscriptionsState );
    }

    int displaySynonymsState	= VsSettings::instance()->value( "displaySynonyms", "Vocabulary" ).toInt();
    if ( displaySynonymsState && displaySynonymsState == Qt::Checked ) {
		ui->chkShowSynonyms->setCheckState( Qt::Checked );
		showSynonyms( displaySynonymsState );
	}
	restoreHeaderSizes(); // From Settings
    ui->tableView->scrollToBottom();

    //emit pModel->dataChanged( pModel->index( 0, 0 ), pModel->index( pModel->rowCount(), 6 ) );

    connect( ui->tableView->horizontalHeader(), SIGNAL( sectionResized ( int, int, int ) ), this, SLOT( saveHeaderSizes( int, int, int ) ) );

    connect( ui->chkShowTranscriptions, SIGNAL( stateChanged( int ) ), this, SLOT( showTranscriptions( int ) ) );
    connect( ui->chkShowSynonyms, SIGNAL( stateChanged( int ) ), this, SLOT( showSynonyms( int ) ) );
    connect( ui->leSearch, SIGNAL( returnPressed() ), ui->btnSearch, SIGNAL( released() ) );
    connect( ui->btnSearch, SIGNAL( released() ), this, SLOT( search() ) );

    connect( ui->treeWidget, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), this, SLOT( showWord( QTreeWidgetItem*, int ) ) );
}

VocabularyWordsWidget::~VocabularyWordsWidget()
{
    delete ui;
}

void VocabularyWordsWidget::initModel()
{
	pModel		= new VocabularyWordsModel();

	// QxModelView module : new feature available to add automatically an empty row at the end of the table to insert quickly new items (setShowEmptyLine() method)
	pModel->setShowEmptyLine( true );

	proxyModel	= new VocabularySortingModel( this );
	proxyModel->setDynamicSortFilter( true );
	proxyModel->setSourceModel( pModel );

	//ui->tableView->setModel( pModel );
	ui->tableView->setModel( proxyModel );
	ui->tableView->setSortingEnabled( true );
	setViewHeader( VsDatabase::instance()->metaInfo() );

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

int VocabularyWordsWidget::insertWord()
{
	int row	= pModel->rowCount( QModelIndex() );
	pModel->insertRow( row );

	return row;
}

/**
 * NOTE: Dont call this function. It is called in VocabularyWidget::loadGroup( int groupId )
 * 		 There is functionality to remember current group into the settings.
 */
void VocabularyWordsWidget::loadGroup( int groupId )
{
	ui->leSearch->setText( "" );	// Clear Search Field
	if ( groupId ) {
		QString query	= QString( "WHERE group_id=%1" ).arg( groupId );
		pModel->qxFetchByQuery( query );
	} else {
		pModel->qxFetchAll();
	}
	ui->stackedWidget->setCurrentWidget( ui->pageVocabulary );

	currentGroup = groupId;

	ui->tableView->scrollToBottom();
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
	int showTranscriptionsState	= ui->chkShowTranscriptions->isChecked() ? Qt::Checked : Qt::Unchecked;

	emit pModel->dataChanged( topLeft, bottomRight );
	initModel();

	loadGroup( currentGroup );
	showTranscriptions( showTranscriptionsState );
	//restoreHeaderSizes();
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
		bool hasId	= pModel->data( topLeft.siblingAtColumn( 0 ) ).toBool();
		if ( hasId ) {
			pModel->setData( topLeft.siblingAtColumn( 4 ), QVariant( currentGroup ) );
			pModel->qxSave();
		} else {
			insertFromEmptyRow( topLeft );

			refreshView( topLeft.siblingAtColumn( 0 ), topLeft.siblingAtColumn( 5 ) );
		}
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

	// Move To Group Actions
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

	// Filter Actions
	menu->addSeparator();
	QAction* actFilter;
	for ( int i = 0; i < TranslationTypesList.count(); i++ ) {
		actFilter	= new QAction( this );
		actFilter->setText( tr( "Filter by" ) + tr( QString( " " + TranslationTypesList[i].toLower() ).toStdString().c_str() ) );
		actFilter->setData( QVariant( i ) );
		//actFilter->setIcon( QIcon( ":/Resources/icons/mail-message-new.svg" ) );
		connect( actFilter, &QAction::triggered, this, &VocabularyWordsWidget::setFilter );

		menu->addAction( actFilter );
	}
	menu->addSeparator();

	// Clear Filters and Sortings
	actFilter	= new QAction( this );
	actFilter->setText( tr( "Clear Filter" ) );
	//actFilter->setIcon( QIcon( ":/Resources/icons/mail-message-new.svg" ) );
	connect( actFilter, &QAction::triggered, this, &VocabularyWordsWidget::clearFilter );
	menu->addAction( actFilter );

	actFilter	= new QAction( this );
	actFilter->setText( tr( "Clear Sorting" ) );
	//actFilter->setIcon( QIcon( ":/Resources/icons/mail-message-new.svg" ) );
	connect( actFilter, &QAction::triggered, this, &VocabularyWordsWidget::clearSorting );
	menu->addAction( actFilter );

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

	QMessageBox::StandardButton reply	= QMessageBox::question(
		this,
		tr( "Delete Vocabulary Words" ),
		tr( "This will erase the selected words. Do you agree?" ),
		QMessageBox::Yes|QMessageBox::No
	);

	if ( reply == QMessageBox::Yes ) {
		for ( int i = 0; i < selectedRows.size(); ++i ) {
			pModel->qxDeleteById( pModel->data( selectedRows[i].siblingAtColumn( 0 ) ) );
		}
		refreshView( selectedRows[0].siblingAtColumn( 0 ), selectedRows[selectedRows.size()-1].siblingAtColumn( 4 ) );
	}
}

void VocabularyWordsWidget::search()
{
	QString searchWord		= ui->leSearch->text();
	QString query			= QString( "WHERE language_1 LIKE '%%1%' OR language_2 LIKE '%%1%'" ).arg( searchWord );

	searchVocabularyModel	= new qx::QxModel<Vocabulary>();
	searchVocabularyModel->qxFetchByQuery( query );

	searchArchiveModel		= new qx::QxModel<ArchiveWord>();
	searchArchiveModel->qxFetchByQuery( query );

	// Clear Previous Searches
	ui->treeWidget->clear();

	// Set Head Titles
	VocabularyMetaInfoPtr metaInfo	= VsDatabase::instance()->metaInfo();
	QStringList headTitles			= viewHeaders( metaInfo );
	QStringList headerLabels;
	headerLabels << "\t" + headTitles.at( 0 );
	if ( ui->chkShowTranscriptions->isChecked() ) {
		headerLabels << headTitles.at( 2 );
	}
	headerLabels << headTitles.at( 1 );
	ui->treeWidget->setHeaderLabels( headerLabels );

	// Init Results
	displayVocabularySearchResults( searchVocabularyModel );
	displayArchiveSearchResults( searchArchiveModel );

	// Display Results
	ui->treeWidget->setColumnWidth( 0 , 200 );
	ui->stackedWidget->setCurrentWidget( ui->pageSearch );
}

void VocabularyWordsWidget::displayVocabularySearchResults( qx::QxModel<Vocabulary>* searchModel )
{
	int groupId;
	QString groupName;
	QMap<int, QTreeWidgetItem*> groups;
	qx::QxModel<VocabularyGroup>* groupModel	= new qx::QxModel<VocabularyGroup>();

	if ( ! searchModel->rowCount() )
		return;

	QTreeWidgetItem* rootItem					= new QTreeWidgetItem( ui->treeWidget );
	rootItem->setText( 0, tr( "Vocabulary" ) );
	rootItem->setIcon( 0, QIcon( ":/Resources/icons/dictionary.svg" ) );
	ui->treeWidget->expandItem( rootItem );

	QTreeWidgetItem* childItem;
	QMap<QString, QVariant> itemData;
	for ( int i = 0; i < searchModel->rowCount(); ++i ) {
		groupId	= searchModel->data( searchModel->index( i, 4 ) ).toInt();
		if ( ! groups.contains( groupId ) ) {
			VocabularyGroupPtr group;
			group.reset( new VocabularyGroup() );
			group->id 	= groupId;
			groupModel->qxFetchById( groupId );
			groupName	= groupModel->data( groupModel->index( 0, 1 ) ).toString();

			groups[groupId]	= new QTreeWidgetItem();
			groups[groupId]->setText( 0, groupName );
			ui->treeWidget->expandItem( groups[groupId] );
			rootItem->addChild( groups[groupId] );
		}

		childItem = new QTreeWidgetItem();
		// Set Item Data
		itemData["where"]	= "Vocabulary";
		itemData["id"]		= searchModel->data( searchModel->index( i, 0 ) );
		childItem->setData( 0, Qt::UserRole, itemData );

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
}

void VocabularyWordsWidget::displayArchiveSearchResults( qx::QxModel<ArchiveWord>* searchModel )
{
	int groupId;
	QString groupName;
	QMap<int, QTreeWidgetItem*> groups;
	qx::QxModel<ArchiveGroup>* groupModel	= new qx::QxModel<ArchiveGroup>();

	if ( ! searchModel->rowCount() )
		return;

	QTreeWidgetItem* rootItem					= new QTreeWidgetItem( ui->treeWidget );
	rootItem->setText( 0, tr( "Archive" ) );
	rootItem->setIcon( 0, QIcon( ":/Resources/icons/archive.svg" ) );
	ui->treeWidget->expandItem( rootItem );

	QTreeWidgetItem* childItem;
	QMap<QString, QVariant> itemData;
	for ( int i = 0; i < searchModel->rowCount(); ++i ) {
		groupId	= searchModel->data( searchModel->index( i, 4 ) ).toInt();
		if ( ! groups.contains( groupId ) ) {
			ArchiveGroupPtr group;
			group.reset( new ArchiveGroup() );
			group->id 	= groupId;
			groupModel->qxFetchById( groupId );
			groupName	= groupModel->data( groupModel->index( 0, 1 ) ).toString();

			groups[groupId]	= new QTreeWidgetItem();
			groups[groupId]->setText( 0, groupName );
			ui->treeWidget->expandItem( groups[groupId] );
			rootItem->addChild( groups[groupId] );
		}

		childItem = new QTreeWidgetItem();

		// Set Item Data
		itemData["where"]	= "Archive";
		itemData["id"]		= searchModel->data( searchModel->index( i, 0 ) );
		childItem->setData( 0, Qt::UserRole, itemData );

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

	VsSettings::instance()->setValue( "displayTranscriptions", QVariant( state ), "Vocabulary" );
}

void VocabularyWordsWidget::showSynonyms( int state )
{
	if ( state == Qt::Checked ) {
		ui->tableView->showColumn( 6 );
	} else {
		ui->tableView->hideColumn( 6 );
	}

	VsSettings::instance()->setValue( "displaySynonyms", QVariant( state ), "Vocabulary" );
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

	if ( state.contains( "displaySynonyms" ) ) {
		ui->chkShowTranscriptions->setChecked( state["displaySynonyms"].toBool() );
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

	// Enable Drag and Drop
	ui->tableView->setDragEnabled( true );
	ui->tableView->setAcceptDrops( true );
	ui->tableView->setDropIndicatorShown( true );
	ui->tableView->setDefaultDropAction( Qt::MoveAction );

	VocabularyTranscriptionsDelegate *transcriptionsDelegate	= new VocabularyTranscriptionsDelegate( ui->tableView );
	ui->tableView->setItemDelegateForColumn( 2, transcriptionsDelegate );

	VocabularyTranslationsDelegate *translationsDelegate	= new VocabularyTranslationsDelegate( ui->tableView );
	ui->tableView->setItemDelegateForColumn( 3, translationsDelegate );

	VocabularySynonymsDelegate *synonymsDelegate	= new VocabularySynonymsDelegate( ui->tableView );
	ui->tableView->setItemDelegateForColumn( 6, synonymsDelegate );

	//ui->tableView->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
	ui->tableView->horizontalHeader()->resizeSections( QHeaderView::ResizeToContents );
	//ui->tableView->horizontalHeader()->setDefaultAlignment( Qt::AlignHCenter  | Qt::AlignVCenter );
	ui->tableView->horizontalHeader()->setStretchLastSection( true );

	connect(
		transcriptionsDelegate,
		SIGNAL( buttonClicked( QModelIndex ) ),
		this,
		SLOT( sayWord( QModelIndex ) )
	);

	connect(
		translationsDelegate,
		SIGNAL( buttonClicked( QModelIndex ) ),
		this,
		SLOT( editTranslationsTypes( QModelIndex ) )
	);

	connect(
		synonymsDelegate,
		SIGNAL( buttonClicked( QModelIndex ) ),
		this,
		SLOT( editSynonyms( QModelIndex ) )
	);

	connect(
		synonymsDelegate,
		SIGNAL( synonymClicked( int ) ),
		this,
		SLOT( showWord( int ) )
	);
}

void VocabularyWordsWidget::showWord( int wordId )
{
	QSqlDatabase db		= qx::QxSqlDatabase::getDatabase();
	QSqlQuery *query	= new QSqlQuery( db );

	QString strQuery	= QString(
			"SELECT v.group_id AS groupId "
			"FROM Vocabulary v "
			"WHERE v.id = %1"
	).arg( wordId );
	query->exec( strQuery );
	query->next();

	showWord( wordId, query->value( "groupId" ).toInt() );
}

void VocabularyWordsWidget::showWord( QTreeWidgetItem* item, int column )
{
	Q_UNUSED( column );

	if ( ! searchVocabularyModel ) {
		return;
	}

	int wordId;
	int groupId;
	QMap<QString, QVariant> itemData	= item->data( 0, Qt::UserRole ).toMap();
	if ( itemData["where"].toString() == "Vocabulary" ) {
		wordId	= itemData["id"].toInt();
		groupId	= 1;
		for ( int i = 0; i < searchVocabularyModel->rowCount(); ++i ) {
			if ( searchVocabularyModel->data( searchVocabularyModel->index( i, 0 ) ).toInt() == wordId ) {
				groupId	= searchVocabularyModel->data( searchVocabularyModel->index( i, 4 ) ).toInt();
				//qDebug() << "Search Model Word Founded. Group ID: " << groupId;
			}
		}

		showWord( wordId, groupId );
	}

	if ( itemData["where"].toString() == "Archive" ) {
		wordId	= itemData["id"].toInt();
		groupId	= 1;
		for ( int i = 0; i < searchArchiveModel->rowCount(); ++i ) {
			if ( searchArchiveModel->data( searchArchiveModel->index( i, 0 ) ).toInt() == wordId ) {
				groupId	= searchArchiveModel->data( searchArchiveModel->index( i, 4 ) ).toInt();
				//qDebug() << "Search Model Word Founded. Group ID: " << groupId;
			}
		}

		showArchiveWord( wordId, groupId );
	}
}

void VocabularyWordsWidget::showWord( int wordId, int groupId )
{
	qobject_cast<VocabularyWidget *>( wdgVocabulary )->setCurrentGroup( groupId );
	//updateView();

	QModelIndex	wordsModelIndex	= QModelIndex();
	for ( int i = 0; i < pModel->rowCount(); ++i ) {
		if ( pModel->data( pModel->index( i, 0 ) ).toInt() == wordId ) {
			wordsModelIndex	= pModel->index( i, 1 );
			//qDebug() << "Words Model Index Found";
		}
	}

	ui->tableView->setCurrentIndex( wordsModelIndex );
	ui->tableView->scrollTo( wordsModelIndex );
}

void VocabularyWordsWidget::showArchiveWord( int wordId, int groupId )
{
	Q_UNUSED( wordId );
	Q_UNUSED( groupId );
}

bool VocabularyWordsWidget::insertFromEmptyRow( QModelIndex index )
{
	VocabularyPtr oVocabulary	= VocabularyPtr( new Vocabulary() );

	oVocabulary->language_1		= pModel->data( index.siblingAtColumn( 1 ) ).toString();
	oVocabulary->language_2		= pModel->data( index.siblingAtColumn( 3 ) ).toString();
	oVocabulary->transcription	= pModel->data( index.siblingAtColumn( 2 ) ).toString();
	oVocabulary->group_id		= currentGroup;

	QSqlError daoError			= qx::dao::insert( oVocabulary );

	return true;
}

void VocabularyWordsWidget::saveHeaderSizes( int logicalIndex, int oldSize, int newSize )
{
	Q_UNUSED( oldSize );

	QMap<QString, QVariant> headerSizes	= VsSettings::instance()->value( "tableHeaderSizes", "Vocabulary" ).toMap();
	headerSizes[QString::number( logicalIndex )]	= QVariant( newSize );

	VsSettings::instance()->setValue( "tableHeaderSizes", headerSizes, "Vocabulary" );
}

void VocabularyWordsWidget::restoreHeaderSizes()
{
	QMap<QString, QVariant> headerSizes	= VsSettings::instance()->value( "tableHeaderSizes", "Vocabulary" ).toMap();
	foreach ( QString key, headerSizes.keys() ) {
		//qDebug() << "Vocabulary Header Size: " << headerSizes[key].toInt();
		ui->tableView->horizontalHeader()->resizeSection( key.toInt(), headerSizes[key].toInt() );
	}
}

void VocabularyWordsWidget::editSynonyms( const QModelIndex &index )
{
	QMap<QString, QVariant> word;
    word["id"]		= index.siblingAtColumn( 0 ).data();
    word["text"]	= index.siblingAtColumn( 1 ).data();

    SynonymsDialog *dlgSynonyms	= new SynonymsDialog( word, this );
    dlgSynonyms->setModal( true );
	if ( dlgSynonyms->exec() == QDialog::Accepted ) {
		//initArchiveWidget();
	}
}

void VocabularyWordsWidget::editTranslationsTypes( const QModelIndex &index )
{
	QMap<QString, QVariant> word;
	word["id"]				= index.siblingAtColumn( 0 ).data();
	word["text"]			= index.siblingAtColumn( 1 ).data();
	word["translations"]	= index.siblingAtColumn( 3 ).data();

	VocabularyTranslationsTypesDialog *dlgTranslations	= new VocabularyTranslationsTypesDialog( word, this );
	dlgTranslations->setModal( true );
	if ( dlgTranslations->exec() == QDialog::Accepted ) {
		//initArchiveWidget();
	}
}

void VocabularyWordsWidget::setFilter()
{
	QAction *action		= qobject_cast<QAction *>( sender() );

	proxyModel->setFilterWordType( action->data().toInt() );
}

void VocabularyWordsWidget::clearFilter()
{
	proxyModel->clearFilter();
}

void VocabularyWordsWidget::clearSorting()
{
	proxyModel->clearSorting();
}
