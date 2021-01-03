#include "QuizListWindow.h"
#include "ui_QuizListWindow.h"

#include <QTreeWidgetItem>
#include <QGroupBox>
#include <QToolButton>
#include <QIcon>
#include <QMessageBox>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "GlobalTypes.h"
#include "Entity/Quiz.h"
#include "Entity/QuizItem.h"

QuizListWindow::QuizListWindow( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::QuizListWindow )
{
    ui->setupUi( this );
    setWindowIcon( QIcon( ":/Resources/icons/quiz-list.svg" ) );

    hideItemColumns = {0, 3};
    initQuizList();
}

QuizListWindow::~QuizListWindow()
{
    delete ui;
}

void QuizListWindow::initQuizList()
{
	// Clears the tree widget by removing all of its items and selections.
	ui->treeWidget->clear();

	int quizId;
	pModel	= new qx::QxModel<Quiz>();
	pModel->qxFetchAll();

	ui->treeWidget->setColumnCount( 2 );
	QTreeWidgetItem *treeItem;
	for ( int i = 0; i < pModel->rowCount(); ++i ) {

		treeItem	= new QTreeWidgetItem( ui->treeWidget );
		quizId		= pModel->data( pModel->index( i, 0 ) ).toInt();

		treeItem->setText( 0, "Quiz " + QString::number( quizId ) );
		initQuizListProperties( treeItem, i );

		ui->treeWidget->setItemWidget( treeItem, 1, quizButtons( quizId ) );
	}
}

QGroupBox* QuizListWindow::quizButtons( int quizId )
{
	QToolButton* button;
	QGroupBox* btnGroup	= new QGroupBox( ui->treeWidget );
	QHBoxLayout* hbox 	= new QHBoxLayout;

	// Display Items Button
	button = new QToolButton();
	button->setIcon( QIcon( ":/Resources/icons/download-later.svg" ) );
	button->setText( "Display Items" );
	button->setProperty( "quizId", QVariant( quizId ) );
	connect( button, SIGNAL( released() ), this, SLOT( displayItems() ) );
	hbox->addWidget( button );

	// Delete Quiz Button
	button = new QToolButton();
	button->setIcon( QIcon( ":/Resources/icons/amarok_cart_remove.svg" ) );
	button->setText( "Delete Quiz" );
	button->setProperty( "quizId", QVariant( quizId ) );
	connect( button, SIGNAL( released() ), this, SLOT( deleteQuiz() ) );
	hbox->addWidget( button );

	// Pack Button group
	hbox->addStretch( 1 );
	btnGroup->setLayout( hbox );

	return btnGroup;
}

void QuizListWindow::initQuizListProperties( QTreeWidgetItem* parent, int quizRow )
{
	QTreeWidgetItem* treeItem;

	EnumDirection direction	= pModel->data( pModel->index( quizRow, 1 ) ).value<EnumDirection>();
	QDateTime startedAt		= pModel->data( pModel->index( quizRow, 5 ) ).toDateTime();
	QDateTime finishedAt	= pModel->data( pModel->index( quizRow, 6 ) ).toDateTime();

	// Date
	treeItem = new QTreeWidgetItem();
	treeItem->setText( 0, "Date" );
	treeItem->setText( 1, pModel->data( pModel->index( quizRow, 5 ) ).toDateTime().toString ( "dd.MM.yyyy" ) );
	parent->addChild( treeItem );

	// Direction
	treeItem = new QTreeWidgetItem();
	treeItem->setText( 0, "Direction" );
	treeItem->setText( 1, ( direction == FIRST_TO_SECOND ) ? "First to Second" : "Second to First" );
	parent->addChild( treeItem );

	// Randomize
	treeItem = new QTreeWidgetItem();
	treeItem->setText( 0, "Randomize" );
	treeItem->setText( 1, pModel->data( pModel->index( quizRow, 2 ) ).toString() );
	parent->addChild( treeItem );

	// Groups
	treeItem = new QTreeWidgetItem();
	treeItem->setText( 0, "Groups" );
	treeItem->setText( 1, pModel->data( pModel->index( quizRow, 3 ) ).toString() );
	parent->addChild( treeItem );

	// Assessment
	treeItem = new QTreeWidgetItem();
	treeItem->setText( 0, "Assessment" );
	treeItem->setText( 1, pModel->data( pModel->index( quizRow, 4 ) ).toString() );
	parent->addChild( treeItem );

	// Duration
	treeItem = new QTreeWidgetItem();
	treeItem->setText( 0, "Duration (s)" );
	treeItem->setText( 1, QString::number( startedAt.msecsTo( finishedAt ) / 1000 ) );
	parent->addChild( treeItem );
}

void QuizListWindow::displayItems()
{
	QToolButton* button	= qobject_cast<QToolButton *>( sender() );
	int quizId			= button->property( "quizId" ).toInt();

	QString query					= QString( "WHERE quiz_id = %1" ).arg( QString::number( quizId ) );
	qx::QxModel<QuizItem>* model	= new qx::QxModel<QuizItem>();
	model->qxFetchByQuery( query );

	ui->tableView->setModel( model );
	for( int i = 0; i < hideItemColumns.size(); i++ ) {
		ui->tableView->hideColumn( hideItemColumns[i] );
	}
}

void QuizListWindow::deleteQuiz()
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(
								this,
								tr( "Delete Quiz" ),
								tr( "This will erase the quiz with all its items. Do you agree?" ),
								QMessageBox::Yes|QMessageBox::No
							);

	if ( reply == QMessageBox::Yes ) {
		QToolButton* button	= qobject_cast<QToolButton *>( sender() );
		int quizId			= button->property( "quizId" ).toInt();

		pModel->qxDeleteByQuery( QString( "WHERE id=%1" ).arg( quizId ) );

		qx::QxModel<QuizItem>* model	= new qx::QxModel<QuizItem>();
		model->qxDeleteByQuery( QString( "WHERE quiz_id=%1" ).arg( quizId ) );

		initQuizList();
	}
}

void QuizListWindow::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}
