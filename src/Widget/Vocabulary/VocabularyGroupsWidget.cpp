#include "VocabularyGroupsWidget.h"
#include "ui_VocabularyGroupsWidget.h"

#include <QListView>
#include <QMenu>
#include <QMessageBox>
//#include <QtTest/QtTest>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "MainWindow.h"
//#include "Entity/VocabularyGroup.h"
#include "Model/VocabularyGroupsModel.h"
#include "View/ViewDelegate/SideBar/SideBarListViewDelegate.h"
#include "VocabularyWidget.h"
#include "Dialog/RenameVocabularyGroupDialog.h"
#include "Dialog/NewVocabularyGroupDialog.h"

VocabularyGroupsWidget::VocabularyGroupsWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::VocabularyGroupsWidget )
{
    ui->setupUi( this );

    initModel();
    initContextMenu();

    ui->listView->setSelectionBehavior( QAbstractItemView::SelectRows );

    /*
	 * Init view delegate
	 */
    SideBarListViewDelegate* itemDelegate	= new SideBarListViewDelegate( pModel, setCurrentGroup(), false, ui->listView );
    ui->listView->setItemDelegate( itemDelegate );

    /*
     * Connect Slots
     */
    //qobject_cast<SideBarListViewDelegate*>( ui->listView->itemDelegate() )
	VocabularyWidget *wdgVocabulary	= qobject_cast<VocabularyWidget *>( parent ); // parent() if not in constructor
	connect( this, SIGNAL( currentGroupChanged( QModelIndex ) ), ui->listView->itemDelegate(), SIGNAL( buttonClicked( QModelIndex ) ) );
	connect( itemDelegate, SIGNAL( buttonClicked( QModelIndex ) ), wdgVocabulary, SLOT( loadGroup( QModelIndex ) ) );
	connect( itemDelegate, SIGNAL( buttonClicked( QModelIndex ) ), this, SLOT( setCurrentGroup( QModelIndex ) ) );

	wdgVocabulary->setCurrentGroupName( ui->listView->model()->data( ui->listView->model()->index( 0, 1 ) ).toString() );
}

VocabularyGroupsWidget::~VocabularyGroupsWidget()
{
    delete ui;
}

int VocabularyGroupsWidget::currentGroup()
{
	return ui->listView->model()->data( ui->listView->model()->index( 0, 0 ) ).toInt();
}

void VocabularyGroupsWidget::initModel()
{
	pModel	= new VocabularyGroupsModel( true );
	pModel->qxFetchAll();
	ui->listView->setModel( pModel );

	connect(
		pModel,
		SIGNAL( rowsInserted( const QModelIndex&, int, int ) ),
		this,
		SLOT( modelRowsInserted( const QModelIndex&, int, int ) )
	);
}

void VocabularyGroupsWidget::initContextMenu()
{
	ui->listView->setSelectionBehavior( QAbstractItemView::SelectItems );
	ui->listView->setSelectionMode( QAbstractItemView::SingleSelection );
	ui->listView->setContextMenuPolicy( Qt::CustomContextMenu );
	connect(
		ui->listView,
		SIGNAL( customContextMenuRequested( QPoint ) ),
		this,
		SLOT( displayContextMenu( QPoint ) )
	);
}

void VocabularyGroupsWidget::refreshView( QModelIndex topLeft, QModelIndex bottomRight )
{
	emit pModel->dataChanged( topLeft, bottomRight );
	initModel();
}

void VocabularyGroupsWidget::refreshView()
{
	initModel();
	repaint();
}

int VocabularyGroupsWidget::groupRow( int groupId )
{
	for ( int i = 0; i < pModel->rowCount(); i++ ) {
		if ( pModel->index( i, 0 ).data().toInt() == groupId ) {
			return i;
		}
	}

	return 0;
}

void VocabularyGroupsWidget::scrollTo( int groupId )
{
	for ( int i = 0; i < pModel->rowCount(); i++ ) {
		if ( pModel->index( i, 0 ).data().toInt() == groupId ) {
			ui->listView->scrollTo( pModel->index( i, 0 ), QAbstractItemView::EnsureVisible );
			//qDebug() << "Scroll To Group ID: " << groupId;
		}
	}
}

void VocabularyGroupsWidget::setCurrentGroup( int groupId )
{
	int currentGroupRow = 0;
	if ( groupId ) {
		currentGroupRow	= groupRow( groupId );
	}

	QModelIndex groupButtonIndex	= pModel->index( currentGroupRow, 1 );
	setCurrentGroup( groupButtonIndex );
	scrollTo( groupId );
	//qDebug() << "Set Current Group: " << currentGroupRow;

	// Ne iska i ne iska be maika mu deeba da smeni butona
	SideBarListViewDelegate* delegate	= qobject_cast<SideBarListViewDelegate*>( ui->listView->itemDelegate() );
	if ( delegate ) {
		delegate->setEvent( QEvent::MouseButtonRelease );
	}
	emit currentGroupChanged( groupButtonIndex );
	refreshView();
}

void VocabularyGroupsWidget::setCurrentGroup( const QModelIndex &index )
{
	ui->listView->setCurrentIndex( index );
	ui->listView->setModel( nullptr );
	ui->listView->setModel( pModel );
}

void VocabularyGroupsWidget::displayContextMenu( QPoint pos )
{
	// This made everything :)
	ui->listView->setCurrentIndex( ui->listView->indexAt( pos ) );

	QMenu *menu		= new QMenu( this );
	QAction *createAct	= new QAction( tr( "&New Group" ), this );
	QAction *renameAct	= new QAction( tr( "&Rename Group" ), this );
	QAction *deleteAct = new QAction( tr( "&Delete Group" ), this );

	createAct->setData( QVariant( pos ) );
	renameAct->setData( QVariant( pos ) );
	deleteAct->setData( QVariant( pos ) );

	connect( createAct, &QAction::triggered, this, &VocabularyGroupsWidget::createGroup );
	connect( renameAct, &QAction::triggered, this, &VocabularyGroupsWidget::renameGroup );
	connect( deleteAct, &QAction::triggered, this, &VocabularyGroupsWidget:: deleteGroup );

	menu->addAction( createAct );
	menu->addAction( renameAct );
	menu->addAction( deleteAct );

	connect(
		menu,
		SIGNAL( aboutToHide() ),
		this,
		SLOT( setCurrentGroup( ui->listView->currentIndex() ) )
	);

	menu->popup( ui->listView->viewport()->mapToGlobal( pos ) );
}

void VocabularyGroupsWidget::createGroup()
{
	QObject *mw	= parent()->parent();
	NewVocabularyGroupDialog *dlg	= qobject_cast<MainWindow*>( mw )->dialogNewVocabularyGroup();
	if( dlg->exec() == QDialog::Accepted )
	{
		refreshView();
	}
	qobject_cast<MainWindow*>( mw )->dialogNewVocabularyGroup();
}

void VocabularyGroupsWidget::renameGroup()
{
	QString oldGroupName	= ui->listView->model()->data( ui->listView->currentIndex().siblingAtColumn( 1 ) ).toString();
	RenameVocabularyGroupDialog *dlg	= new RenameVocabularyGroupDialog( oldGroupName, this );

	dlg->setModal( true );
	if( dlg->exec() == QDialog::Accepted )
	{
		ui->listView->model()->setData( ui->listView->currentIndex().siblingAtColumn( 1 ), QVariant( dlg->newName() ) );
		pModel->qxSave();
	}
}

void VocabularyGroupsWidget::deleteGroup()
{
	QMessageBox::StandardButton reply	= QMessageBox::question(
		this,
		tr( "Delete Vocabulary Group" ),
		tr( "This will erase all Words associated with this Group. Do you agree?" ),
		QMessageBox::Yes|QMessageBox::No
	);

	if ( reply == QMessageBox::Yes ) {
		QModelIndex indexId				= ui->listView->currentIndex().siblingAtColumn( 0 );
		//VocabularyWidget *wdgVocabulary	= qobject_cast<VocabularyWidget *>( parent() );
		//wdgVocabulary->deleteGroup( pModel->data( indexId ).toInt() );

		qx::QxModel<Vocabulary>* vocModel	= new qx::QxModel<Vocabulary>;
		QString query	= QString( "WHERE group_id=%1" ).arg( QString::number( pModel->data( indexId ).toInt() ) );
		vocModel->qxDeleteByQuery( query );

		pModel->qxDeleteById( pModel->data( indexId ) );

		refreshView();

		/**
		 * @Todo: Refreshing the widgets is very hard:
		 * 			QObject::connect: Cannot connect SideBarListViewDelegate::buttonClicked( QModelIndex ) to (nullptr)::loadGroup( QModelIndex )
		 */
//		qDebug() << QString::number( currentGroup() );
//		wdgVocabulary->refreshWidgets();
	}
}

void VocabularyGroupsWidget::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}

void VocabularyGroupsWidget::modelRowsInserted( const QModelIndex & parent, int start, int end )
{
	Q_UNUSED( parent );
	Q_UNUSED( start );
	Q_UNUSED( end );

	//ui->listView->scrollTo( pModel->index( start, 0 ) );
	ui->listView->scrollToBottom();
}

int VocabularyGroupsWidget::setCurrentGroup()
{
	int currentGroupRow = 0;
	int currentGroup	= VsSettings::instance()->value( "currentGroup", "Vocabulary" ).toInt();
	if ( currentGroup ) {
		currentGroupRow	= groupRow( currentGroup );
	}
	setCurrentGroup( currentGroup );

	return currentGroupRow;
}
