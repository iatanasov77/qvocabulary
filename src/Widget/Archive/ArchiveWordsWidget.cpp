#include "ArchiveWordsWidget.h"

#include <QApplication>
#include <QTableView>
#include <QMenu>
#include <QTextToSpeech>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Application/VsSettings.h"
#include "Application/VsDatabase.h"
#include "Application/VsSpeaker.h"
#include "Entity/ArchiveWord.h"
#include "Entity/ArchiveGroup.h"

#include "Widget/Archive/ArchiveWidget.h"
#include "Model/Helper.h"
#include "View/ViewDelegate/Vocabulary/VocabularyTranscriptionsDelegate.h"

ArchiveWordsWidget::ArchiveWordsWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::VocabularyWordsWidget )
{
    ui->setupUi( this );
    initView();
    wdgVocabulary	= parent;

    //QVBoxLayout *tableLayout	= new QVBoxLayout( ui->tableView );

    currentGroup 	= 1;
    hideColumns 	= {0, 4};

    initModel();
    initTextToSpeech();

    setViewHeader( VsDatabase::instance()->metaInfo() );
    restoreHeaderSizes(); // From Settings
    ui->tableView->scrollToBottom();
    ui->chkShowTranscriptions->hide();
    for( int i = 0; i < hideColumns.size(); i++ ) {
		ui->tableView->hideColumn( hideColumns[i] );
	}

    connect( ui->tableView->horizontalHeader(), SIGNAL( sectionResized ( int, int, int ) ), this, SLOT( saveHeaderSizes( int, int, int ) ) );

    connect( ui->leSearch, SIGNAL( returnPressed() ), ui->btnSearch, SIGNAL( released() ) );
    connect( ui->btnSearch, SIGNAL( released() ), this, SLOT( search() ) );
    connect( ui->treeWidget, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), this, SLOT( showWord( QTreeWidgetItem*, int ) ) );
}

ArchiveWordsWidget::~ArchiveWordsWidget()
{
    delete ui;
}

void ArchiveWordsWidget::initModel()
{
	pModel	= new qx::QxModel<ArchiveWord>();
	ui->tableView->setModel( pModel );
}

void ArchiveWordsWidget::initTextToSpeech()
{
	speeker = new VsSpeaker( this );
}

void ArchiveWordsWidget::setViewHeader( VocabularyMetaInfoPtr metaInfo )
{
	QStringList headTitles	= viewHeaders( metaInfo );

	pModel->setHeaderData( 1, Qt::Horizontal, headTitles.at( 0 ), Qt::DisplayRole );
	pModel->setHeaderData( 2, Qt::Horizontal, headTitles.at( 2 ), Qt::DisplayRole );
	pModel->setHeaderData( 3, Qt::Horizontal, headTitles.at( 1 ), Qt::DisplayRole );
	pModel->setHeaderData( 5, Qt::Horizontal, headTitles.at( 3 ), Qt::DisplayRole );
}

QStringList ArchiveWordsWidget::viewHeaders( VocabularyMetaInfoPtr metaInfo )
{
	QStringList headTitles;
	headTitles
		<< qApp->translate( "Vocabulary", metaInfo->language1.toStdString().c_str() )
		<< qApp->translate( "Vocabulary", metaInfo->language2.toStdString().c_str() )
		<< qApp->translate( "Vocabulary", "Transcription" )
		<< qApp->translate( "Vocabulary", "Description" );

	return headTitles;
}

/**
 * NOTE: Dont call this function. It is called in VocabularyWidget::loadGroup( int groupId )
 * 		 There is functionality to remember current group into the settings.
 */
void ArchiveWordsWidget::loadGroup( int groupId )
{
	ui->leSearch->setText( "" );	// Clear Serch Field
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

int ArchiveWordsWidget::deleteGroup( int groupId )
{
	QString query	= QString( "WHERE group_id=%1" ).arg( QString::number( groupId ) );
	pModel->qxDeleteByQuery( query );

	if ( currentGroup == groupId ) {
		currentGroup = groupId - 1;
		loadGroup( currentGroup );
	}

	return currentGroup;
}

void ArchiveWordsWidget::refreshView( QModelIndex topLeft, QModelIndex bottomRight )
{
	Q_UNUSED( topLeft );
	Q_UNUSED( bottomRight );

	initModel();
	setViewHeader( VsDatabase::instance()->metaInfo() );

	loadGroup( currentGroup );
}

void ArchiveWordsWidget::updateView()
{
	initModel();
	loadGroup( currentGroup );
}

void ArchiveWordsWidget::search()
{
	searchModel			= new qx::QxModel<ArchiveWord>();
	QString searchWord	= ui->leSearch->text();
	QString query		= QString( "WHERE language_1 LIKE '%%1%' OR language_2 LIKE '%%1%'" ).arg( searchWord );
	searchModel->qxFetchByQuery( query );

	displaySearchResults( searchModel );
}

void ArchiveWordsWidget::displaySearchResults( qx::QxModel<ArchiveWord>* searchModel )
{
	int groupId;
	QString groupName;
	QMap<int, QTreeWidgetItem*> groups;
	qx::QxModel<ArchiveGroup>* groupModel	= new qx::QxModel<ArchiveGroup>();

	ui->treeWidget->clear();

	QTreeWidgetItem* childItem;
	QString translation;
	VocabularyMetaInfoPtr metaInfo	= VsDatabase::instance()->metaInfo();
	QStringList headTitles			= viewHeaders( metaInfo );

	QStringList headerLabels;
	headerLabels << "\t" + headTitles.at( 0 );
	headerLabels << headTitles.at( 2 );
	headerLabels << headTitles.at( 1 );
	ui->treeWidget->setHeaderLabels( headerLabels );

	for ( int i = 0; i < searchModel->rowCount(); ++i ) {
		groupId	= searchModel->data( searchModel->index( i, 4 ) ).toInt();
		if ( ! groups.contains( groupId ) ) {
			ArchiveGroupPtr group;
			group.reset( new ArchiveGroup() );
			group->id 	= groupId;
			groupModel->qxFetchById( groupId );
			groupName	= groupModel->data( groupModel->index( 0, 1 ) ).toString();

			groups[groupId]	= new QTreeWidgetItem( ui->treeWidget );
			groups[groupId]->setText( 0, groupName );
			ui->treeWidget->expandItem( groups[groupId] );
		}

		childItem = new QTreeWidgetItem();

		childItem->setText( 0, searchModel->data( searchModel->index( i, 1 ) ).toString() );
		childItem->setData( 0, Qt::UserRole, searchModel->data( searchModel->index( i, 0 ) ) );

		// Transcriptions
		QFont font;
		font.setBold( true );
		childItem->setText( 1, searchModel->data( searchModel->index( i, 2 ) ).toString() );
		childItem->setFont( 1, font );

		translation	= searchModel->data( searchModel->index( i, 3 ) ).toString();
		childItem->setText( 2, translation );
		qDebug() << "Search Translation: " << translation;

		groups[groupId]->addChild( childItem );
	}

	// Resize Columns
	ui->treeWidget->setColumnWidth( 0 , 200 );

	ui->stackedWidget->setCurrentWidget( ui->pageSearch );
}

void ArchiveWordsWidget::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}

void ArchiveWordsWidget::sayWord( const QModelIndex &index )
{
    QString word	= index.siblingAtColumn( 1 ).data().toString();

    speeker->tts()->say( word );
}

void ArchiveWordsWidget::updateSpeaker()
{
	speeker->updateTts();
}

void ArchiveWordsWidget::initView()
{
	VocabularyTranscriptionsDelegate *transcriptionsDelegate	= new VocabularyTranscriptionsDelegate( ui->tableView );
	ui->tableView->setItemDelegateForColumn( 2, transcriptionsDelegate );
	//ui->tableView->setItemDelegate( itemDelegate );

	//ui->tableView->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
	ui->tableView->horizontalHeader()->resizeSections( QHeaderView::ResizeToContents );
	ui->tableView->horizontalHeader()->setStretchLastSection( true );

	ui->tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
	ui->tableView->setSelectionMode( QAbstractItemView::ExtendedSelection );

	// Disable Editing
	ui->tableView->setEditTriggers( QAbstractItemView::NoEditTriggers );

	connect(
		transcriptionsDelegate,
		SIGNAL( buttonClicked( QModelIndex ) ),
		this,
		SLOT( sayWord( QModelIndex ) )
	);
}

void ArchiveWordsWidget::showWord( QTreeWidgetItem* item, int column )
{
	Q_UNUSED( column );

	if ( ! searchModel ) {
		return;
	}

	int wordId						= item->data( 0, Qt::UserRole ).toInt();
	int groupId						= 1;
	for ( int i = 0; i < searchModel->rowCount(); ++i ) {
		searchModel->data( searchModel->index( i, 0 ) ).toInt();
		if ( searchModel->data( searchModel->index( i, 0 ) ).toInt() == wordId ) {
			groupId	= searchModel->data( searchModel->index( i, 4 ) ).toInt();
			//qDebug() << "Search Model Word Founded. Group ID: " << groupId;
		}
	}

	showWord( wordId, groupId );
}

void ArchiveWordsWidget::showWord( int wordId, int groupId )
{
	qobject_cast<ArchiveWidget *>( wdgVocabulary )->setCurrentGroup( groupId );
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

void ArchiveWordsWidget::saveHeaderSizes( int logicalIndex, int oldSize, int newSize )
{
	Q_UNUSED( oldSize );

	QMap<QString, QVariant> headerSizes	= VsSettings::instance()->value( "tableHeaderSizes", "Archive" ).toMap();
	headerSizes[QString::number( logicalIndex )]	= QVariant( newSize );

	VsSettings::instance()->setValue( "tableHeaderSizes", headerSizes, "Archive" );
}

void ArchiveWordsWidget::restoreHeaderSizes()
{
	QMap<QString, QVariant> headerSizes	= VsSettings::instance()->value( "tableHeaderSizes", "Archive" ).toMap();
	foreach ( QString key, headerSizes.keys() ) {
		//qDebug() << "Vocabulary Header Size: " << headerSizes[key].toInt();
		ui->tableView->horizontalHeader()->resizeSection( key.toInt(), headerSizes[key].toInt() );
	}
}
