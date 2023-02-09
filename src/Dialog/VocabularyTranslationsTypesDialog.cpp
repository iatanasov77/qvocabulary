#include "VocabularyTranslationsTypesDialog.h"
#include "ui_VocabularyTranslationsTypesDialog.h"

#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QProgressBar>
#include <QSpinBox>
#include <QMessageBox>

#include "GlobalTypes.h"

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyGroup.h"

#include "Widget/VsTableHeaderView.h"

VocabularyTranslationsTypesDialog::VocabularyTranslationsTypesDialog( QMap<QString, QVariant> word, QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::VocabularyTranslationsTypesDialog )
{
    ui->setupUi( this );
    resize( 1024, 400 );

    _word			= word;
    _wordTypes		= QList<QButtonGroup *>();
    _wordWeights	= QList<QSpinBox *>();
    _defaultWeight	= 4;

    ui->lblWord->setText( _word["text"].toString() );
    initTranslations( _word );

    QPushButton *saveButton = ui->buttonBox->button( QDialogButtonBox::Save );
    saveButton->setText( tr( "Save" ) );
    connect( saveButton, SIGNAL( clicked() ), this, SLOT( save() ) );

    // May be there is other way to translate Core Strings
    ui->buttonBox->button( QDialogButtonBox::Cancel )->setText( tr( "Cancel" ) );

    connect( ui->tableWidget->horizontalHeader(), SIGNAL( checkboxClicked( int, bool ) ), this, SLOT( checkAll( int, bool ) ) );
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
	ui->tableWidget->setColumnCount( 8 );

	VsTableHeaderView *myHeader = new VsTableHeaderView( Qt::Horizontal, ui->tableWidget, {1, 2, 3, 4, 5, 6} );
	ui->tableWidget->setHorizontalHeader( myHeader );
	ui->tableWidget->setHorizontalHeaderLabels( {
		tr( "Word" ),
		tr( "Noun" ),
		tr( "Adjective" ),
		tr( "Verb" ),
		tr( "Adverb" ),
		tr( "Preposition" ),
		tr( "Conjunction" ),
		tr( "Weight" )
	} );

	foreach( QString trWord, translations ) {
		ui->tableWidget->setItem( row, 0, new QTableWidgetItem( trWord.trimmed() ) );

		row++;
	}

	initTypes( translations.size() );
	initWeights( translations.size() );
}

void VocabularyTranslationsTypesDialog::initTypes( int countRows )
{
	for ( int y = 0; y < countRows; y++ ) {
		QString word	= ui->tableWidget->item( y, 0 )->text();
		_wordTypes << new QButtonGroup( this );
		_wordTypes[y]->setExclusive( true );
		for ( int x = 1; x < 7; x++ ) {
			int checkedId			= x - 1;
			QWidget *checkBoxWidget = new QWidget();
			QHBoxLayout *layout		= new QHBoxLayout( checkBoxWidget );
			QRadioButton *checkbox	= new QRadioButton();
			checkbox->setChecked( getCheckedId( word ) == checkedId );
			_wordTypes[y]->addButton( checkbox, checkedId );

			layout->addWidget( checkbox );
			layout->setAlignment( Qt::AlignCenter );  // Center the checkbox
			layout->setContentsMargins( 0, 0, 0, 0 );
			ui->tableWidget->setCellWidget( y, x, checkBoxWidget );
		}
	}
}

void VocabularyTranslationsTypesDialog::initWeights( int countRows )
{
	for ( int y = 0; y < countRows; y++ ) {
		QString word					= ui->tableWidget->item( y, 0 )->text();
		int wordWeigth					= getWordWeight( word );
		QProgressBar *weightIndicator	= new QProgressBar( this );
		weightIndicator->setTextVisible( false );
		weightIndicator->setFixedHeight( 7 );
		weightIndicator->setRange( 1, 4 );
		weightIndicator->setValue( wordWeigth );
		_wordWeights << new QSpinBox( this );
		_wordWeights[y]->setRange( 1, 4 );
		_wordWeights[y]->setValue( wordWeigth );

		QWidget *weightWidget = new QWidget();
		QHBoxLayout *layout		= new QHBoxLayout( weightWidget );
		layout->addWidget( weightIndicator );
		layout->addWidget( _wordWeights[y] );
		layout->setAlignment( Qt::AlignCenter );
		layout->setContentsMargins( 0, 0, 0, 0 );
		ui->tableWidget->setCellWidget( y, 7, weightWidget );

		connect( _wordWeights[y], SIGNAL( valueChanged( int ) ), weightIndicator, SLOT( setValue( int ) ) );
	}
}

bool VocabularyTranslationsTypesDialog::checkWordTypes()
{
	foreach( QButtonGroup *btnGroup, _wordTypes ) {
		int checkedId	= btnGroup->checkedId();
		if ( checkedId == -1 ) { // No Type Checked
			QMessageBox::critical(
				this,
				tr( "Error" ),
				tr( "There are not selected types for some words !" )
			);

			return false;
		}
	}

	return true;
}

void VocabularyTranslationsTypesDialog::save()
{
	if ( ! checkWordTypes() ) {
		setResult( QDialog::Rejected );
		return;
	}

	int row				= 0;
	QString strQuery	= "";
	QSqlQuery query		= QSqlQuery( qx::QxSqlDatabase::getDatabase() );

	foreach( QButtonGroup *btnGroup, _wordTypes ) {
		int checkedId	= btnGroup->checkedId();
		QString trWord	= ui->tableWidget->item( row, 0 )->text();
		int trWeight	= _wordWeights[row]->value();
		if ( getCheckedId( trWord ) >= 0 ) {
			strQuery	+= QString( "UPDATE VocabularyWordTranslation "
									"SET tr_type='%1', tr_weight=%2 "
									"WHERE word_id=%3 AND tr_word='%4';" )
									.arg( TranslationTypesList[checkedId] )
									.arg( trWeight )
									.arg( _word["id"].toInt() )
									.arg( trWord );
		} else {
			strQuery	+= QString( "INSERT INTO VocabularyWordTranslation"
									"( word_id, tr_word, tr_type, tr_weight ) "
									"VALUES( %1, '%2', '%3', %4 );" )
									.arg( _word["id"].toInt() )
									.arg( trWord )
									.arg( TranslationTypesList[checkedId] )
									.arg( trWeight );
		}

		row++;
	}

	if ( strQuery.length() ) {
		QStringList scriptQueries = strQuery.split( ';' );
		foreach ( QString queryTxt, scriptQueries ) {
			if ( queryTxt.trimmed().isEmpty() ) {
				continue;
			}

			//qDebug() << queryTxt;
			if ( ! query.exec( queryTxt ) ) {
				qDebug() << query.lastError().text();
			}
			query.finish();
		}
	}
}

int VocabularyTranslationsTypesDialog::getCheckedId( QString trWord )
{
	QSqlQuery query		= QSqlQuery( qx::QxSqlDatabase::getDatabase() );
	QString strQuery	= QString( "SELECT tr_type FROM VocabularyWordTranslation "
									"WHERE word_id=%1 AND tr_word='%2'" )
									.arg( _word["id"].toInt() )
									.arg( trWord );

	//qDebug() << strQuery;
	query.exec( strQuery );
	if ( query.first() )
		return TranslationTypesList.indexOf( query.value( "tr_type" ).toString() );
	else
		return -1;
}

int VocabularyTranslationsTypesDialog::getWordWeight( QString trWord )
{
	QSqlQuery query		= QSqlQuery( qx::QxSqlDatabase::getDatabase() );
	QString strQuery	= QString( "SELECT tr_weight FROM VocabularyWordTranslation "
									"WHERE word_id=%1 AND tr_word='%2'" )
									.arg( _word["id"].toInt() )
									.arg( trWord );

	qDebug() << strQuery;
	query.exec( strQuery );
	if ( query.first() )
		return query.value( "tr_weight" ).toInt();
	else
		return _defaultWeight;
}

void VocabularyTranslationsTypesDialog::checkAll( int logicalIndex, bool isOn )
{
	//qDebug() << "VocabularyTranslationsTypesDialog::checkAll: " << isOn;
	foreach ( QButtonGroup *btnGroup, _wordTypes ) {
		if ( isOn ) {
			btnGroup->button( logicalIndex - 1 )->setChecked( isOn );
		} else {
			//btnGroup->button( logicalIndex - 1 )->setAutoExclusive( false );
			btnGroup->button( logicalIndex - 1 )->setChecked( false );
			//btnGroup->button( logicalIndex - 1 )->setAutoExclusive( true );
		}
	}
}
