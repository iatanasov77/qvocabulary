#include "VocabularyTranslationsTypesDialog.h"
#include "ui_VocabularyTranslationsTypesDialog.h"

#include <QPushButton>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyGroup.h"

VocabularyTranslationsTypesDialog::VocabularyTranslationsTypesDialog( QMap<QString, QVariant> word, QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::VocabularyTranslationsTypesDialog )
{
    ui->setupUi( this );
    ui->lblWord->setText( word["text"].toString() );
    initTranslations( word );
    _word	= word;

    QPushButton *saveButton = ui->buttonBox->button( QDialogButtonBox::Save );
    saveButton->setText( tr( "Save" ) );
    connect( saveButton, SIGNAL( clicked() ), this, SLOT( save() ) );

    // May be there is other way to translate Core Strings
    ui->buttonBox->button( QDialogButtonBox::Cancel )->setText( tr( "Cancel" ) );
}

VocabularyTranslationsTypesDialog::~VocabularyTranslationsTypesDialog()
{
    delete ui;
}

void VocabularyTranslationsTypesDialog::initTranslations( QMap<QString, QVariant> word )
{
	int row						= 0;
	QStringList translations	= word["translations"].toString().split( "," );
	ui->tableWidget->setRowCount( translations.size() );
	ui->tableWidget->setColumnCount( 5 );
	ui->tableWidget->setHorizontalHeaderLabels( {
		tr( "Word" ),
		tr( "Noun" ),
		tr( "Adjective" ),
		tr( "Verb" ),
		tr( "Proverb" )
	} );

	foreach( QString trWord, translations ) {
		ui->tableWidget->setItem( row, 0, new QTableWidgetItem( trWord.trimmed() ) );
		row++;
	}
}

void VocabularyTranslationsTypesDialog::save()
{

}
