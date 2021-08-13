#include "ArchiveWidget.h"

#include <QListView>
#include <QTableView>
#include <QHeaderView>
#include <QSplitter>
#include <QMenu>
#include <QSettings>
#include <QMap>
#include <QVariant>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Application/VsDatabase.h"
#include "Application/VsSettings.h"
#include "Entity/ArchiveWord.h"
#include "Entity/ArchiveGroup.h"
#include "ModelView/SideBarListViewDelegate.h"

#include "Widget/Archive/ArchiveGroupsWidget.h"
#include "Widget/Archive/ArchiveWordsWidget.h"

ArchiveWidget::ArchiveWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::VocabularyWidget )
{
    ui->setupUi( this );

    init();
    initModels();

    // Hide Current group Display in Header Frame
    for( int i = 0; i < ui->horizontalLayout_3->count(); ++i ) {
        ui->horizontalLayout_3->itemAt(i)->widget()->hide();
    }

    // Load Current Group by settings or first loaded
    int currentGroup	= VsSettings::instance()->value( "currentGroup", "Archive" ).toInt();
    if ( currentGroup ) {
    	setCurrentGroup( currentGroup );
    } else {
    	loadGroup( wdgGroups->currentGroup() );
    }

    // Scroll to selected Group
    wdgGroups->scrollTo( currentGroup );
}

ArchiveWidget::~ArchiveWidget()
{
    delete ui;
}

void ArchiveWidget::init()
{
	/*
	 * Init widgets in the QSplitter
	 */
	QSplitter* horizSplitter 	= new QSplitter( Qt::Horizontal );
	wdgGroups					= new ArchiveGroupsWidget( this );
	wdgWords					= new ArchiveWordsWidget( this );

	wdgGroups->setMaximumWidth( 400 );
	wdgGroups->adjustSize();

	horizSplitter->insertWidget( 0, wdgGroups );
	horizSplitter->insertWidget( 1, wdgWords );

	/*
 	 * Styling QSplitter
 	 */
	const QString splitterSheet =  \
		"QSplitter::handle:horizontal { \
			border: 1px solid black; \
			margin: 0px; \
			padding: 0px; \
		}";
	horizSplitter->setStyleSheet( splitterSheet );

	ui->horizontalLayout->addWidget( horizSplitter );
}

void ArchiveWidget::initModels()
{
	VocabularyMetaInfoPtr metaInfo	= VsDatabase::instance()->metaInfo();
	if ( metaInfo ) {
		ui->databaseLabel->setText( tr( "Archive" ) );
		QString trWords	= qApp->translate( "VocabularyWidget", "words" );
		ui->databaseName->setText( QString( "%1 ( %2 %3 )" )
									.arg( metaInfo->name )
									.arg( QString::number( qx::dao::count<ArchiveWord>() ) )
									.arg( trWords )
								);
	}
}

void ArchiveWidget::setCurrentGroupName( QString groupName )
{
	ui->groupName->setText( groupName );
}

void ArchiveWidget::loadGroup( int groupId )
{
	currentGroup		= groupId;
	wdgWords->loadGroup( groupId );

	VsSettings::instance()->setValue( "currentGroup", currentGroup, "Archive" );
}

void ArchiveWidget::loadGroup( const QModelIndex &index )
{
    int groupId			= index.siblingAtColumn( 0 ).data().toInt();
    QString groupName	= index.siblingAtColumn( 1 ).data().toString();

	setCurrentGroupName( groupName );
    loadGroup( groupId );
}

void ArchiveWidget::refreshWidgets()
{
	init();
}

void ArchiveWidget::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}

void ArchiveWidget::updateSpeaker()
{
	wdgWords->updateSpeaker();
}

void ArchiveWidget::setCurrentGroup( int groupId )
{
	loadGroup( groupId );
	wdgGroups->setCurrentGroup( groupId );
	//wdgGroups->repaint();
}

void ArchiveWidget::showWord( int wordId, int groupId )
{
	wdgWords->showWord( wordId, groupId );
}
