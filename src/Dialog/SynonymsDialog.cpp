#include "SynonymsDialog.h"
#include "ui_SynonymsDialog.h"

#include <QPushButton>
#include <QErrorMessage>
#include <QCompleter>
#include <QSortFilterProxyModel>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "GlobalTypes.h"
#include "Entity/ArchiveGroup.h"
#include "Entity/ArchiveWord.h"

#include "Application/VsApplication.h"
#include "Application/VsDatabase.h"
#include "ModelView/VocabularyWordsModel.h"

SynonymsDialog::SynonymsDialog( int wordId, QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::SynonymsDialog )
{
    ui->setupUi( this );

    vww = parent;	// parent VocabularyWordsWidget
	_wordId	= wordId;

    initVocabularyCombo();
    initArchiveCombo();

    QPushButton *saveButton = ui->buttonBox->button( QDialogButtonBox::Save );
    saveButton->setText( tr( "Save" ) );
    connect( saveButton, SIGNAL( clicked() ), this, SLOT( saveSynonyms() ) );
}

SynonymsDialog::~SynonymsDialog()
{
    delete ui;
}

void SynonymsDialog::saveSynonyms()
{
	//saveVocabularySynonyms();
	saveArchiveSynonyms();
}

void SynonymsDialog::initVocabularyCombo()
{
	int id;
	QString word;
	cmbVocabulary	= new MultiSelectComboBox( this );
	cmbVocabulary->setDisplayText( tr( "-- Select Vocabulary Words --" ) );

	VocabularyWordsModel *model	= new VocabularyWordsModel();
	model->qxFetchAll();
	QList<int> synonyms			= getVocabularySynonyms();
	for( int r = 0; r < model->rowCount(); ++r ) {
		id 		= model->data( model->index( r, 0 ) ).toInt();
		word	= model->data( model->index( r, 1 ) ).toString();

		cmbVocabulary->addItem( word, id );
		cmbVocabulary->setItemData( r, synonyms.contains( id ), Qt::CheckStateRole );
	}

//	QCompleter *completear					= new QCompleter( this );
//	QSortFilterProxyModel *completerModel	= new QSortFilterProxyModel( this );
//	completerModel->setSourceModel( model );
//	completerModel->setFilterCaseSensitivity( Qt::CaseInsensitive );
//	completerModel->setFilterKeyColumn( 1 );
//
//	completear->setCaseSensitivity( Qt::CaseInsensitive );
//	completear->setModel( completerModel );
//	completear->setCompletionColumn( 1 );
//	completear->setCompletionMode( QCompleter::PopupCompletion );
//	completear->setFilterMode( Qt::MatchStartsWith );
//
//	cmbVocabulary->setEditable( true );
//	cmbVocabulary->setCompleter( completear );

	ui->formLayout->replaceWidget( ui->cbVocabularyWords, cmbVocabulary );
	ui->cbVocabularyWords->hide();	// Should Remove It
}

void SynonymsDialog::initArchiveCombo()
{
	int id;
	QString word;
	cmbArchive	= new MultiSelectComboBox( this );
	cmbArchive->setDisplayText( tr( "-- Select Archive Words --" ) );

	qx::QxModel<ArchiveWord> *model	= new qx::QxModel<ArchiveWord>();
	model->qxFetchAll();
	QList<int> synonyms				= getArchiveSynonyms();
	for( int r = 0; r < model->rowCount(); ++r ) {
		id 		= model->data( model->index( r, 0 ) ).toInt();
		word	= model->data( model->index( r, 1 ) ).toString();

		cmbArchive->addItem( word, id );
		cmbArchive->setItemData( r, synonyms.contains( id ), Qt::CheckStateRole );
	}

	ui->formLayout->replaceWidget( ui->cbArchiveWords, cmbArchive );
	ui->cbArchiveWords->hide();	// Should Remove It
}

void SynonymsDialog::selectedVocabularyWords()
{
	QStringList words;
	for ( int i = 0; i < cmbVocabulary->count(); i++ ) {
		if ( cmbVocabulary->itemData( i, Qt::CheckStateRole ).toBool() ) {
			QString word	= cmbVocabulary->itemText( i );
			qDebug() << "Checked Word: " << word;
			words << word;
		}
	}
}

void SynonymsDialog::selectedArchiveWords()
{

}

QList<int> SynonymsDialog::getVocabularySynonyms()
{
	QList<int> synonyms;

	QString strQuery	= QString( "SELECT * FROM VocabularyWordSynonym WHERE word_id=%1 AND target = '%2'" )
								.arg( _wordId )
								.arg( SynonymTargets["VOCABULARY"] );
	QSqlQuery query( strQuery, qx::QxSqlDatabase::getDatabase() );
	while( query.next() ) {
		synonyms << query.value( "synonym_id" ).toInt();
	}

	return synonyms;
}

QList<int> SynonymsDialog::getArchiveSynonyms()
{
	QList<int> synonyms;

	QSqlQuery query(
		QString( "SELECT * FROM VocabularyWordSynonym WHERE word_id=%1 AND target = '%2'" )
			.arg( _wordId )
			.arg( SynonymTargets["ARCHIVE"] )
		,  qx::QxSqlDatabase::getDatabase()
	);
	while( query.next() ) {
		synonyms << query.value( "synonym_id" ).toInt();
	}

	return synonyms;
}

void SynonymsDialog::saveVocabularySynonyms()
{
	QString strQuery;
	QSqlQuery query;

	selectedVocabularyWords();

	query.prepare( "UPDATE one SET d1 = d1 + :d WHERE ..." );
	//query.bindValue(...);
	query.exec();
	if ( query.numRowsAffected() == 0 ) {
	    query.prepare( "INSERT ..." );
	    //query.bindValue(...);
	    query.exec();
	}
}

void SynonymsDialog::saveArchiveSynonyms()
{

}
