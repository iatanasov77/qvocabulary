#include "VocabularyGroupsWidget.h"
#include "ui_VocabularyGroupsWidget.h"

#include <QListView>
#include <QMenu>
#include <QMessageBox>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyGroup.h"
#include "ModelView/SideBarListViewDelegate.h"
#include "VocabularyWidget.h"
#include "Dialog/RenameVocabularyGroupDialog.h"

VocabularyGroupsWidget::VocabularyGroupsWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::VocabularyGroupsWidget )
{
    ui->setupUi( this );

    initModel();
    initContextMenu();

	/*
	 * Init view delegate
	 */
    SideBarListViewDelegate* itemDelegate	= new SideBarListViewDelegate( ui->listView );
    ui->listView->setItemDelegate( itemDelegate );

    /*
     * Connect Slots
     */
	VocabularyWidget *wdgVocabulary	= qobject_cast<VocabularyWidget *>( parent ); // parent() if not in constructor
	connect( itemDelegate, SIGNAL( buttonClicked( QModelIndex ) ), wdgVocabulary, SLOT( loadGroup( QModelIndex ) ) );
	connect( itemDelegate, SIGNAL( buttonClicked( QModelIndex ) ), this, SLOT( setCurrentGroup( QModelIndex ) ) );

	wdgVocabulary->setCurrentGroupName( ui->listView->model()->data( ui->listView->model()->index( 0, 1 ) ).toString() );
}

VocabularyGroupsWidget::~VocabularyGroupsWidget()
{
    delete ui;
}

void VocabularyGroupsWidget::initModel()
{
	pModel	= new qx::QxModel<VocabularyGroup>();
	pModel->qxFetchAll();
	ui->listView->setModel( pModel );
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
}

void VocabularyGroupsWidget::setCurrentGroup( const QModelIndex &index )
{
	ui->listView->setCurrentIndex( index );
}

void VocabularyGroupsWidget::displayContextMenu( QPoint pos )
{
	QModelIndex currentIndex	= ui->listView->currentIndex();

	// This made everything :)
	ui->listView->setCurrentIndex( ui->listView->indexAt( pos ) );

	QMenu *menu		= new QMenu( this );
	QAction *renameAct	= new QAction( tr( "&Rename Group" ), this );
	QAction *deleteAct = new QAction( tr( "&Delete Group" ), this );

	renameAct->setData( QVariant( pos ) );
	deleteAct->setData( QVariant( pos ) );

	connect( renameAct, &QAction::triggered, this, &VocabularyGroupsWidget::renameGroup );
	connect( deleteAct, &QAction::triggered, this, &VocabularyGroupsWidget:: deleteGroup );

	menu->addAction( renameAct );
	menu->addAction( deleteAct );

	connect(
		menu,
		SIGNAL( aboutToHide() ),
		this,
		SLOT( setCurrentGroup( currentIndex ) )
	);

	menu->popup( ui->listView->viewport()->mapToGlobal( pos ) );
}

void VocabularyGroupsWidget::renameGroup()
{
	RenameVocabularyGroupDialog *dlg	= new RenameVocabularyGroupDialog( this );

	dlg->setModal( true );
	if( dlg->exec() == QDialog::Accepted )
	{
		ui->listView->model()->setData( ui->listView->currentIndex().siblingAtColumn( 1 ), QVariant( dlg->newName() ) );
		pModel->qxSave();
	}
}

void VocabularyGroupsWidget::deleteGroup()
{
	QModelIndex indexId				= ui->listView->currentIndex().siblingAtColumn( 0 );
	int groupId						= pModel->data( indexId ).toInt();
	VocabularyWidget *wdgVocabulary	= qobject_cast<VocabularyWidget *>( parent() );

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question( this, "Delete Group",
									"This will erase all Words associated with this Group. Do you agree?",
									QMessageBox::Yes|QMessageBox::No
								);
	if ( reply == QMessageBox::Yes ) {
		//wdgVocabulary->deleteGroup( groupId );
		pModel->qxDeleteById( pModel->data( indexId ) );

		//refreshView( indexId, indexId.siblingAtColumn( 1 ) );
		refreshView( );
	}
}
