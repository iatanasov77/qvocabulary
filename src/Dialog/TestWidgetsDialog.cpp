#include "TestWidgetsDialog.h"
#include "ui_TestWidgetsDialog.h"

#include <QTableView>

#include "../lib/VankoSoft/Widget/VsIconLabel.h"

#include "Model/VocabularyWordsModel.h"
#include "View/ViewDelegate/Test/TestDelegate.h"

TestWidgetsDialog::TestWidgetsDialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::TestWidgetsDialog )
{
    ui->setupUi( this );
    mw = parent;

    testWidgets();

    connect( ui->btnExec, SIGNAL( clicked() ), this, SLOT( execSomething() ) );
    connect( ui->btnClose, SIGNAL( clicked() ), this, SLOT( closeDialog() ) );

    ui->buttonBox->hide();
}

TestWidgetsDialog::~TestWidgetsDialog()
{
    delete ui;
}

void TestWidgetsDialog::execSomething()
{

}

void TestWidgetsDialog::closeDialog()
{
	reject();
}

void TestWidgetsDialog::testWidgets()
{
	//testIconLabel();
	testIconWidgetsDelegate();
}

void TestWidgetsDialog::testIconLabel()
{
	ui->verticalLayout_2->addWidget( new VsIconLabel( ":/Resources/icons/dictionary.svg", "Test VsIconLabel" ) );
}

void TestWidgetsDialog::testIconWidgetsDelegate()
{
	VocabularyWordsModel *pModel;
	QTableView	*tableView;
	TestDelegate *delegate;
	QList<int> hideColumns 	= {0, 2, 3, 4, 5};

	// Init View
	tableView	= new QTableView( this );
	ui->verticalLayout_2->addWidget( tableView );

	// Init Delegate
	delegate	= new TestDelegate( tableView );
	tableView->setItemDelegateForColumn( 6, delegate );

	// Init Model
	pModel	= new VocabularyWordsModel();
	pModel->qxFetchByQuery( "WHERE group_id=8" );
	tableView->setModel( pModel );

	for( int i = 0; i < hideColumns.size(); i++ ) {
		tableView->hideColumn( hideColumns[i] );
	}
}
