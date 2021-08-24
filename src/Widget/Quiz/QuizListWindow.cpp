#include "QuizListWindow.h"
#include "ui_QuizListWindow.h"

#include <QTreeWidgetItem>
#include <QGroupBox>
#include <QToolButton>
#include <QIcon>
#include <QMessageBox>
#include <QCursor>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "GlobalTypes.h"
#include "Entity/Quiz.h"
#include "Entity/QuizItem.h"
#include "Application/VsSettings.h"

QuizListWindow::QuizListWindow( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::QuizListWindow )
{
    ui->setupUi( this );
    setWindowIcon( QIcon( ":/Resources/icons/quiz-list.svg" ) );

    ui->quizTitle->hide();
    hideItemColumns = {0, 3};
    initQuizList();

    // Set QuizListWindow Size Previous Set In Settings
	VsSettings* settings	= VsSettings::instance();
	int qwWidth			= settings->value( "quizListWindowWidth", "Quiz" ).toInt();
	int qwHeight		= settings->value( "quizListWindowHeight", "Quiz" ).toInt();
	if ( qwWidth && qwHeight ) {
		resize( qwWidth, qwHeight );
	}

	if ( ! qwWidth ) {
		qwWidth = 800;
	}
	// 35% | 65%
	int leftWidth	= 35.0 / 100 * qwWidth;
	int rightWidth	= 65.0 / 100 * qwWidth;
	ui->splitter->setSizes( { leftWidth, rightWidth } );
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
	QString quizTitle;
	QMap<QString, QVariant> properties;
	pModel	= new qx::QxModel<Quiz>();
	pModel->qxFetchAll();

	ui->treeWidget->setColumnCount( 2 );
	ui->treeWidget->setHeaderLabels( { tr( "Quiz" ), tr( "Assessment" ), tr( "Date" ), tr( "Attributes" ) } );

	QTreeWidgetItem *treeItem;
	for ( int i = 0; i < pModel->rowCount(); ++i ) {

		treeItem				= new QTreeWidgetItem( ui->treeWidget );
		quizId					= pModel->data( pModel->index( i, 0 ) ).toInt();
		quizTitle				= "Quiz " + QString::number( quizId );
		properties["quizId"]	= QVariant( quizId );
		properties["quizTitle"]	= QVariant( quizTitle );

		initQuizListItem( treeItem, i, properties );
		ui->treeWidget->setItemWidget( treeItem, 3, quizButtons( properties ) );
	}
}

QGroupBox* QuizListWindow::quizButtons( QMap<QString, QVariant> properties )
{
	QGroupBox* btnGroup	= new QGroupBox( ui->treeWidget );
	QHBoxLayout* hbox 	= new QHBoxLayout;

	// Display Items Button
	hbox->addWidget( createToolButton(
		tr( "Display Quiz" ),
		QIcon( ":/Resources/icons/download-later.svg" ),
		SLOT( displayItems() ),
		properties
	));

	// Delete Quiz Button
	hbox->addWidget( createToolButton(
		tr( "Delete Quiz" ),
		QIcon( ":/Resources/icons/amarok_cart_remove.svg" ),
		SLOT( deleteQuiz() ),
		properties
	));

	// Pack Button group
	hbox->addStretch( 1 );
	btnGroup->setLayout( hbox );

	return btnGroup;
}

void QuizListWindow::initQuizListItem( QTreeWidgetItem* parent, int quizRow, QMap<QString, QVariant> properties )
{
	QString assessment		= pModel->data( pModel->index( quizRow, 4 ) ).toString();
	QString date			= pModel->data( pModel->index( quizRow, 5 ) ).toDateTime().toString ( "dd.MM.yyyy" );

	QFont itemFont			= QFont( "" , 7 , QFont::Bold );
	QBrush itemBrush;
	switch ( assessment.toInt() ) {
		case 3:
		case 4:
			itemBrush	= QBrush( Qt::yellow );
			break;
		case 5:
		case 6:
			itemBrush	= QBrush( Qt::green );
			break;
		default:
			itemBrush	= QBrush( Qt::red );
	}

	parent->setForeground( 0 , itemBrush );
	parent->setFont( 0,  itemFont );
	parent->setForeground( 1 , itemBrush );
	parent->setFont( 1,  itemFont );
	parent->setForeground( 2 , itemBrush );
	parent->setFont( 2,  itemFont );

	parent->setText( 0, properties["quizTitle"].toString() );
	parent->setText( 1, assessment );
	parent->setText( 2, date );

	initQuizListDetails( parent, quizRow );
}

void QuizListWindow::initQuizListDetails( QTreeWidgetItem* parent, int quizRow )
{
	EnumDirection direction	= pModel->data( pModel->index( quizRow, 1 ) ).value<EnumDirection>();
	QDateTime startedAt		= pModel->data( pModel->index( quizRow, 5 ) ).toDateTime();
	QDateTime finishedAt	= pModel->data( pModel->index( quizRow, 6 ) ).toDateTime();

	QTreeWidgetItem* treeItem;

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
	QString quizTitle	= button->property( "quizTitle" ).toString();

	QString query					= QString( "WHERE quiz_id = %1" ).arg( QString::number( quizId ) );
	qx::QxModel<QuizItem>* model	= new qx::QxModel<QuizItem>();
	model->qxFetchByQuery( query );

	ui->quizTitle->setText( quizTitle );
	ui->quizTitle->show();

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

void QuizListWindow::resizeEvent( QResizeEvent* event )
{
	QWidget::resizeEvent( event );

	VsSettings* settings	= VsSettings::instance();
	QSize qwSize		= event->size(); // Returns the new size of the widget. This is the same as QWidget::size().

	settings->setValue( "quizListWindowWidth", QVariant( qwSize.width() ), "Quiz" );
	settings->setValue( "quizListWindowHeight", QVariant( qwSize.height() ), "Quiz" );
}

QToolButton* QuizListWindow::createToolButton( const QString &toolTip, const QIcon &icon, const char *member, QMap<QString, QVariant> properties )
{
    QToolButton *button = new QToolButton( this );
    button->setToolTip( toolTip );
    button->setIcon( icon );
    button->setIconSize( QSize( 24, 24 ) );
    button->setCursor( QCursor( Qt::PointingHandCursor ) );

    foreach ( QString key, properties.keys() ) {
    	button->setProperty( key.toStdString().c_str(), properties[key] );
    }

    connect( button, SIGNAL( clicked() ), this, member );

    return button;
}
