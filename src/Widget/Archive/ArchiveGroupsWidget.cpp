#include "ArchiveGroupsWidget.h"

#include <QListView>
#include <QMessageBox>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/ArchiveGroup.h"
#include "ModelView/SideBarListViewDelegate.h"
#include "ArchiveWidget.h"

ArchiveGroupsWidget::ArchiveGroupsWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::VocabularyGroupsWidget )
{
    ui->setupUi( this );

    initModel();

    ui->listView->setSelectionBehavior( QAbstractItemView::SelectRows );

    int currentGroupRow = 0;

	/*
	 * Init view delegate
	 */
    SideBarListViewDelegate* itemDelegate	= new SideBarListViewDelegate( currentGroupRow, ui->listView );
    ui->listView->setItemDelegate( itemDelegate );

    /*
     * Connect Slots
     */
    ArchiveWidget *wdgVocabulary	= qobject_cast<ArchiveWidget *>( parent ); // parent() if not in constructor
	connect( this, SIGNAL( currentGroupChanged( QModelIndex ) ), itemDelegate, SIGNAL( buttonClicked( QModelIndex ) ) );
	connect( itemDelegate, SIGNAL( buttonClicked( QModelIndex ) ), wdgVocabulary, SLOT( loadGroup( QModelIndex ) ) );
	connect( itemDelegate, SIGNAL( buttonClicked( QModelIndex ) ), this, SLOT( setCurrentGroup( QModelIndex ) ) );

	wdgVocabulary->setCurrentGroupName( ui->listView->model()->data( ui->listView->model()->index( 0, 1 ) ).toString() );
}

ArchiveGroupsWidget::~ArchiveGroupsWidget()
{
    delete ui;
}

int ArchiveGroupsWidget::currentGroup()
{
	return ui->listView->model()->data( ui->listView->model()->index( 0, 0 ) ).toInt();
}

void ArchiveGroupsWidget::initModel()
{
	pModel	= new qx::QxModel<ArchiveGroup>();
	pModel->qxFetchAll();
	ui->listView->setModel( pModel );

	connect(
		pModel,
		SIGNAL( rowsInserted( const QModelIndex&, int, int ) ),
		this,
		SLOT( modelRowsInserted( const QModelIndex&, int, int ) )
	);
}

int ArchiveGroupsWidget::groupRow( int groupId )
{
	for ( int i = 0; i < pModel->rowCount(); i++ ) {
		if ( pModel->index( i, 0 ).data().toInt() == groupId ) {
			return i;
		}
	}

	return 0;
}

void ArchiveGroupsWidget::scrollTo( int groupId )
{
	for ( int i = 0; i < pModel->rowCount(); i++ ) {
		if ( pModel->index( i, 0 ).data().toInt() == groupId ) {
			ui->listView->scrollTo( pModel->index( i, 0 ), QAbstractItemView::EnsureVisible );
			//qDebug() << "Scroll To Group ID: " << groupId;
		}
	}
}

void ArchiveGroupsWidget::setCurrentGroup( int groupId )
{
	int currentGroupRow = 0;
	if ( groupId ) {
		currentGroupRow	= groupRow( groupId );
	}
}

void ArchiveGroupsWidget::setCurrentGroup( const QModelIndex &index )
{
	ui->listView->setCurrentIndex( index );
	ui->listView->setModel( nullptr );
	ui->listView->setModel( pModel );
}

void ArchiveGroupsWidget::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}
