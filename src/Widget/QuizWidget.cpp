#include "QuizWidget.h"
#include "ui_QuizWidget.h"

#include <QTableView>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyMetaInfo.h"
#include "Entity/Vocabulary.h"
#include "Entity/VocabularyGroup.h"

#include "ModelView/QuizViewDelegate.h"
#include "ModelView/QuizItemModel.h"

QuizWidget::QuizWidget( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::QuizWidget )
{
    ui->setupUi( this );

    currentGroup = 1;
    hideColumns = {0, 3};

    initModel();
    loadGroup( currentGroup );

    connect(
		ui->btnNextQuestion,
		SIGNAL( released() ),
		this,
		SLOT( insertWord() )
	);
}

QuizWidget::~QuizWidget()
{
    delete ui;
}

QPushButton* QuizWidget::btnStopQuiz()
{
	return ui->btnStopQuiz;
}

void QuizWidget::initModel()
{
	pModelVocabulary	= new qx::QxModel<Vocabulary>();
	pModelVocabulary->qxFetchAll();

	//pModel				= new qx::QxModel<QuizItem>();
	pModel				= new QuizItemModel();

	ui->tableView->setModel( pModel );
	ui->tableView->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
	for( int i = 0; i < hideColumns.size(); i++ ) {
		ui->tableView->hideColumn( hideColumns[i] );
	}

//	QuizViewDelegate* itemDelegate	= new QuizViewDelegate( ui->tableView );
//	ui->tableView->setItemDelegate( itemDelegate );

	//ui->tableView->item( row,col )->setFlags( Qt::ItemIsSelectable|Qt::ItemIsEnabled );
}

void QuizWidget::setViewHeader( VocabularyMetaInfoPtr metaInfo )
{
	pModel->setHeaderData( 1, Qt::Horizontal, metaInfo->language1, Qt::DisplayRole );
	pModel->setHeaderData( 2, Qt::Horizontal, metaInfo->language2, Qt::DisplayRole );
}

void QuizWidget::insertWord()
{
	QString word	= randomWord();
	int targetRow	= pModel->rowCount( QModelIndex() );

	pModel->insertRow( targetRow );
	pModel->setData( pModel->index( targetRow, 1 ), QVariant( word ) );
}

void QuizWidget::loadGroup( int groupId )
{
//	QString query	= QString( "WHERE group_id=%1" ).arg( groupId );
//	pModel->qxFetchByQuery( query );
//
//	currentGroup = groupId;
}

QString QuizWidget::randomWord()
{
	int randomRow	= qrand() % ( ( pModelVocabulary->rowCount() + 1 ) - 0 ) + 0;

	return pModelVocabulary->data( pModelVocabulary->index( randomRow, 1 ) ).toString();
}

void QuizWidget::onDataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
	pModel->setData( topLeft.siblingAtColumn( 3 ), QVariant( currentGroup ) );
	pModel->qxSave();


	//QRegExp rx( "^\\d\\d?$" );
	/*
	QRegExp rx( "(?i)\b" + word + "\b" );
	bool found	= word.indexOf ( rx ) >= 0;
	*/

}
