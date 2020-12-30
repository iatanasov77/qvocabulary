#include "SettingsWidgetGeneral.h"
#include "ui_SettingsWidgetGeneral.h"

#include <QDebug>

SettingsWidgetGeneral::SettingsWidgetGeneral( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::SettingsWidgetGeneral )
{
    ui->setupUi( this );

    init();
}

SettingsWidgetGeneral::~SettingsWidgetGeneral()
{
    delete ui;
}

void SettingsWidgetGeneral::init()
{
	// addItem(const QString &text, const QVariant &userData = QVariant())
	ui->cmbLanguage->addItem( "English" );
	ui->cmbLanguage->addItem( "Bulgarian" );
}
