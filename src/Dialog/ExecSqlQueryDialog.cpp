#include "ExecSqlQueryDialog.h"
#include "ui_ExecSqlQueryDialog.h"

#include <QPushButton>
#include <QFileDialog>
#include <QErrorMessage>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

ExecSqlQueryDialog::ExecSqlQueryDialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::ExecSqlQueryDialog )
{
    ui->setupUi( this );
    ui->errorFrame->hide();
    mw = parent;

    ui->lblErrorMessage->setStyleSheet( "QLabel { color : red; }" );

    connect( ui->btnExec, SIGNAL( clicked() ), this, SLOT( execSqlQuery() ) );
    connect( ui->btnClose, SIGNAL( clicked() ), this, SLOT( closeDialog() ) );

    ui->buttonBox->hide();
}

ExecSqlQueryDialog::~ExecSqlQueryDialog()
{
    delete ui;
}

void ExecSqlQueryDialog::execSqlQuery()
{
	ui->errorFrame->hide(); // Hide Previous Error if sexists

	QString strQuery	= ui->teQuery->toPlainText();
	if ( strQuery.length() ) {
		QSqlQuery query( strQuery, qx::QxSqlDatabase::getDatabase() );
		if ( query.lastError().isValid() ) {
			ui->lblErrorMessage->setText( query.lastError().text() );
			ui->errorFrame->show();
		}
	}
}

void ExecSqlQueryDialog::closeDialog()
{
	reject();
}
