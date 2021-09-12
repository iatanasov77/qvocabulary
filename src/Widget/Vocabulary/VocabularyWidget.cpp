#include "VocabularyWidget.h"
#include "ui_VocabularyWidget.h"

#include <QListView>
#include <QTableView>
#include <QHeaderView>
#include <QSplitter>
#include <QMenu>
#include <QSettings>
#include <QMap>
#include <QVariant>
#include <QMessageBox>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Model/Helper.h"
#include "Application/VsDatabase.h"
#include "Application/VsSettings.h"
#include "Entity/VocabularyMetaInfo.h"
#include "Entity/Vocabulary.h"
#include "Entity/VocabularyGroup.h"
#include "View/ViewDelegate/SideBar/SideBarListViewDelegate.h"

#include "Widget/Vocabulary/VocabularyGroupsWidget.h"
#include "Widget/Vocabulary/VocabularyWordsWidget.h"

VocabularyWidget::VocabularyWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::VocabularyWidget )
{
    ui->setupUi( this );

    init();
    Helper::delay( 5 ); // Add a delay to database to be loaded
    initModels();

    // Hide Current group Display in Header Frame
    for( int i = 0; i < ui->horizontalLayout_3->count(); ++i ) {
        ui->horizontalLayout_3->itemAt(i)->widget()->hide();
    }

    // Load Current Group by settings or first loaded
    int currentGroup	= VsSettings::instance()->value( "currentGroup", "Vocabulary" ).toInt();
    if ( currentGroup ) {
    	setCurrentGroup( currentGroup );
    } else {
    	loadGroup( wdgGroups->currentGroup() );
    }

    // Scroll to selected Group
    wdgGroups->scrollTo( currentGroup );
}

VocabularyWidget::~VocabularyWidget()
{
	VsSettings::instance()->setValue( "splitterSizes", horizSplitter->saveState(), "Vocabulary" );

	delete wdgWords;
	delete wdgGroups;
	delete horizSplitter;
    delete ui;
}

void VocabularyWidget::init()
{
	/*
	 * Init widgets in the QSplitter
	 */
	horizSplitter 	= new QSplitter( Qt::Horizontal );
	wdgGroups		= new VocabularyGroupsWidget( this );
	wdgWords		= new VocabularyWordsWidget( this );

	wdgGroups->setMaximumWidth( 400 );
	wdgGroups->adjustSize();

	horizSplitter->insertWidget( 0, wdgGroups );
	horizSplitter->insertWidget( 1, wdgWords );

	ui->horizontalLayout->addWidget( horizSplitter );

	QByteArray	splitterSizes	= VsSettings::instance()->value( "splitterSizes", "Vocabulary" ).toByteArray();
	horizSplitter->restoreState( splitterSizes );
}

void VocabularyWidget::initModels()
{
	//QMessageBox::warning( this, "Meta Info Entered", "Meta Info Entered." );
	ui->lblDatabase->setText( tr( "Vocabulary: " ) );
	ui->lblDatabaseWordsCount->setText( QString( "( %1 %2 )" )
												.arg( QString::number( qx::dao::count<Vocabulary>() ) )
												.arg( qApp->translate( "VocabularyWidget", "words" ) )
											);
	// Init VocabularyMetaInfo
	VocabularyMetaInfoPtr metaInfo	= VsDatabase::instance()->metaInfo();
	if ( metaInfo ) {
		ui->lblDatabaseName->setText( QString( "%1" ).arg( metaInfo->name ) );
		ui->lblDatabaseName->setFont( QFont( "Arial", 12, QFont::Bold ) );
	}
}

void VocabularyWidget::setCurrentGroupName( QString groupName )
{
	ui->groupName->setText( groupName );
}

void VocabularyWidget::insertWord()
{
	wdgWords->insertWord();
	setWordsCount();
}

void VocabularyWidget::loadGroup( int groupId )
{
	currentGroup		= groupId;
	wdgWords->loadGroup( groupId );

	wdgWords->updateView();	// This is the key to not show an one more empty row for an empty group
							// when switch from another not empty group.

	VsSettings::instance()->setValue( "currentGroup", currentGroup, "Vocabulary" );
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

void VocabularyWidget::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}

void VocabularyWidget::updateSpeaker()
{
	wdgWords->updateSpeaker();
}

void VocabularyWidget::setCurrentGroup( int groupId )
{
	loadGroup( groupId );
	wdgGroups->setCurrentGroup( groupId );
}

QMap<QString, QVariant> VocabularyWidget::getState()
{
	QMap<QString, QVariant> widgetState;

	// Can to be merged with states of other widgets if needed
	widgetState	= wdgWords->getState();
	//qDebug() << "DEBUG WIDGET STATE: " << widgetState["showTranscriptions"].toString();

	return widgetState;
}

void VocabularyWidget::setState( QMap<QString, QVariant> state )
{
	wdgWords->setState( state );
}

void VocabularyWidget::showWord( int wordId, int groupId )
{
	wdgWords->showWord( wordId, groupId );
}

void VocabularyWidget::refreshGroups()
{
	wdgGroups->refreshView();
}

void VocabularyWidget::setWordsCount()
{
	ui->lblDatabaseWordsCount->setText(  QString( "( %1 %2 )" )
											.arg( QString::number( qx::dao::count<Vocabulary>() ) )
											.arg( qApp->translate( "VocabularyWidget", "words" ) )
										);
}
