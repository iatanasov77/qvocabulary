#include "ArchiveCompareWidget.h"
#include "ui_ArchiveCompareWidget.h"

#include <QSqlQuery>

#include "MainWindow.h"

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "ModelView/ArchiveCompareTableView.h"

ArchiveCompareWidget::ArchiveCompareWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::ArchiveCompareWidget )
{
	mw	= parent;
    ui->setupUi( this );

    initModel();
    initView();

    connect( ui->tableView, SIGNAL( clicked( QModelIndex ) ), this, SLOT( setSelection( QModelIndex ) ) );
    connect( ui->tableView, SIGNAL( doubleClicked( QModelIndex ) ), this, SLOT( setSelection( QModelIndex ) ) );
	connect( ui->tableView, SIGNAL( doubleClicked( QModelIndex ) ), this, SLOT( openWord( QModelIndex ) ) );
}

ArchiveCompareWidget::~ArchiveCompareWidget()
{
    delete ui;
}

void ArchiveCompareWidget::initView()
{
	ui->verticalLayout->removeWidget( ui->tableView );
	ui->tableView	= new ArchiveCompareTableView( this );
	ui->verticalLayout->addWidget( ui->tableView );

	ui->tableView->setModel( pModel );
	hideColumns	= {4, 5, 6, 7};
	for( int i = 0; i < hideColumns.size(); i++ ) {
		ui->tableView->hideColumn( hideColumns[i] );
	}
}

void ArchiveCompareWidget::initModel()
{
	QString strCompairQuery	= QString( "SELECT VG.name AS VocabularyGroup, VW.language_1 AS VocabularyWord, AG.name AS ArchiveGroup, AW.language_1 AS ArchiveWord"
	    										", VG.id as VgId, VW.id as VwId, AG.id as AgId, AW.id as AwId "
	    										"FROM Vocabulary VW "
	    										"LEFT JOIN VocabularyGroup VG ON VW.group_id = VG.id "
	    										"LEFT JOIN ArchiveWord AW ON VW.language_1 = AW.language_1 "
	    										"LEFT JOIN ArchiveGroup AG ON AW.group_id = AG.id "
	    										"WHERE AwId IS NOT NULL" );
	QSqlQuery compairQuery	= QSqlQuery( strCompairQuery, qx::QxSqlDatabase::getDatabase() );
	compairQuery.exec();

	pModel					= new QSqlQueryModel();
	pModel->setQuery( compairQuery );
}

void ArchiveCompareWidget::setSelection( QModelIndex index )
{
	QItemSelection selectedItems;

	if ( index.column() == 0 || index.column() == 1 ) {
		selectedItems	= QItemSelection( index.siblingAtColumn( 0 ), index.siblingAtColumn( 1 ) );
	}
	if ( index.column() == 2 || index.column() == 3 ) {
		selectedItems	= QItemSelection( index.siblingAtColumn( 2 ), index.siblingAtColumn( 3 ) );
	}

	ui->tableView->selectionModel()->clearSelection();
	ui->tableView->selectionModel()->select( selectedItems, QItemSelectionModel::Select );
}

void ArchiveCompareWidget::openWord( QModelIndex index )
{
	if ( index.column() == 0 || index.column() == 1 ) {
		int groupId	= pModel->data( index.siblingAtColumn( 4 ) ).toInt();
		int wordId	= pModel->data( index.siblingAtColumn( 5 ) ).toInt();
		qobject_cast<MainWindow*>( mw )->showVocabularyWord( wordId, groupId );
	}

	if ( index.column() == 2 || index.column() == 3 ) {
		int archiveGroupId	= pModel->data( index.siblingAtColumn( 6 ) ).toInt();
		int archiveWordId	= pModel->data( index.siblingAtColumn( 7 ) ).toInt();
		qobject_cast<MainWindow*>( mw )->showArchiveWord( archiveWordId, archiveGroupId  );
	}
}
