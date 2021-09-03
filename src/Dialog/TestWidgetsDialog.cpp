#include "TestWidgetsDialog.h"
#include "ui_TestWidgetsDialog.h"

#include "../lib/VankoSoft/Widget/VsIconLabel.h"

TestWidgetsDialog::TestWidgetsDialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::TestWidgetsDialog )
{
    ui->setupUi( this );
    mw = parent;

    ui->verticalLayout_2->addWidget( new VsIconLabel( ":/Resources/icons/dictionary.svg", "Test VsIconLabel" ) );

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
