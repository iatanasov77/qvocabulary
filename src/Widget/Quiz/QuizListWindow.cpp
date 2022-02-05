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
#include "View/ViewDelegate/Quiz/QuizItemModelDelegate.h"

QuizListWindow::QuizListWindow( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::QuizListWindow )
{
    ui->setupUi( this );
    setWindowIcon( QIcon( ":/Resources/icons/quiz-list.svg" ) );

    ui->quizTitle->hide();
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
		SLOT( openQuiz() ),
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
	int quizId						= pModel->data( pModel->index( quizRow, 0 ) ).toInt();
	QString countQuestionsQuery		= QString( "WHERE quiz_id = %1" ).arg( QString::number( quizId ) );
	QString countRightAnswersQuery	= QString( "WHERE quiz_id = %1 AND right_answer = TRUE" ).arg( QString::number( quizId ) );
	int countQuestions				= qx::dao::count<QuizItem>( countQuestionsQuery );
	int rightAnswers				= qx::dao::count<QuizItem>( countRightAnswersQuery );
	QString assessment				= QString( "%1 ( %2/%3 )" )
										.arg( pModel->data( pModel->index( quizRow, 4 ) ).toString() )
										.arg( QString::number( rightAnswers ) )
										.arg( QString::number( countQuestions ) );

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
	treeItem->setText( 0, tr( "Direction" ) );
	treeItem->setText( 1, ( direction == FIRST_TO_SECOND ) ?
			qApp->translate( "QuizListWindow", "First to Second" ) :
			qApp->translate( "QuizListWindow", "Second to First" )
	);
	parent->addChild( treeItem );

	// Randomize
	treeItem = new QTreeWidgetItem();
	treeItem->setText( 0, tr( "Randomize" ) );
	treeItem->setText( 1, pModel->data( pModel->index( quizRow, 2 ) ).toString() );
	parent->addChild( treeItem );

	// Groups
	treeItem = new QTreeWidgetItem();
	treeItem->setText( 0, tr( "Groups" ) );
	treeItem->setText( 1, "" );
	QJsonArray groups	= QJsonDocument::fromJson( pModel->data( pModel->index( quizRow, 3 ) ).toByteArray() ).array();
	QTreeWidgetItem* groupItem;
	for ( int i = 0; i < groups.count(); ++i ) {
		groupItem	= new QTreeWidgetItem();
		groupItem->setText( 0, "" );
		groupItem->setText( 1, groups[i].toString() );
		treeItem->addChild( groupItem );
	}
	parent->addChild( treeItem );

	// Duration
	QTime duration( 0,0,0 );
	duration = duration.addSecs( startedAt.msecsTo( finishedAt ) / 1000 );

	treeItem = new QTreeWidgetItem();
	treeItem->setText( 0, tr( "Duration" ) );
	treeItem->setText( 1, duration.toString( "hh:mm:ss" ) );
	parent->addChild( treeItem );
}

QSortFilterProxyModel* QuizListWindow::createItemsModel( int quizId )
{
	QString query					= QString( "WHERE quiz_id = %1" ).arg( QString::number( quizId ) );
	qx::QxModel<QuizItem> *model	= new qx::QxModel<QuizItem>();
	model->qxFetchByQuery( query );

	QSortFilterProxyModel *proxy	= new QSortFilterProxyModel( this );
	proxy->setDynamicSortFilter( true );
	proxy->setSourceModel( model );

	return proxy;
}

void QuizListWindow::openQuiz()
{
	QToolButton* button			= qobject_cast<QToolButton *>( sender() );
	int quizId					= button->property( "quizId" ).toInt();
	QString quizTitle			= button->property( "quizTitle" ).toString();

	displayItems( quizId, quizTitle );
}

void QuizListWindow::displayItems( int quizId, QString quizTitle )
{
	QStringList itemHeadTitles	= quizItemHeaders();
	QSortFilterProxyModel *model= createItemsModel( quizId );

	ui->quizTitle->setText( quizTitle );
	ui->quizTitle->show();
	ui->tableView->setModel( model );
	ui->tableView->setSortingEnabled( true );

	model->setHeaderData( 2, Qt::Horizontal, itemHeadTitles.at( 0 ), Qt::DisplayRole );
	model->setHeaderData( 3, Qt::Horizontal, itemHeadTitles.at( 1 ), Qt::DisplayRole );
	model->setHeaderData( 4, Qt::Horizontal, itemHeadTitles.at( 2 ), Qt::DisplayRole );
	model->setHeaderData( 5, Qt::Horizontal, itemHeadTitles.at( 3 ), Qt::DisplayRole );
	model->setHeaderData( 6, Qt::Horizontal, itemHeadTitles.at( 4 ), Qt::DisplayRole );
	model->setHeaderData( 7, Qt::Horizontal, itemHeadTitles.at( 5 ), Qt::DisplayRole );
	model->setHeaderData( 8, Qt::Horizontal, itemHeadTitles.at( 6 ), Qt::DisplayRole );

	QList<QVariant> hideItemColumns	= VsSettings::instance()->value( "displayItemColumns", "Quiz" ).toList();
	for( int i = 0; i < hideItemColumns.size(); i++ ) {
		if ( ! hideItemColumns[i].toBool() ) {
			ui->tableView->hideColumn( i );
		}
	}

	QuizItemModelDelegate* itemDelegate	= new QuizItemModelDelegate( ui->tableView, 5, true, false );
	ui->tableView->setItemDelegateForColumn( 5, itemDelegate );
	//ui->tableView->setItemDelegate( itemDelegate );

	ui->tableView->setEditTriggers( QAbstractItemView::NoEditTriggers );	// Quiz Items to be not Editable
	ui->tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
	ui->tableView->setSelectionMode( QAbstractItemView::SingleSelection );
}

void QuizListWindow::deleteQuiz()
{
	QMessageBox messageBox(
		QMessageBox::Question,
		tr( "Delete Quiz" ),
		tr( "This will erase the quiz with all its items. Do you agree?" ),
		QMessageBox::Yes | QMessageBox::No,
		this
	);
	messageBox.setButtonText( QMessageBox::Yes, tr( "Yes" ) );
	messageBox.setButtonText( QMessageBox::No, tr( "No" ) );

	if ( messageBox.exec() == QMessageBox::Yes ) {
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

QStringList QuizListWindow::quizItemHeaders()
{
	QStringList headTitles;
	headTitles
		<< qApp->translate( "Quiz", "Word" )
		<< qApp->translate( "Quiz", "Translation" )
		<< qApp->translate( "Quiz", "Answer" )
		<< qApp->translate( "Quiz", "Right" )

		<< qApp->translate( "Vocabulary", "Transcription" )
		<< qApp->translate( "Vocabulary", "Description" )
		<< qApp->translate( "Vocabulary", "Synonyms" );

	return headTitles;
}

