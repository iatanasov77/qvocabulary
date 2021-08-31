#include "SynonymsDialog.h"
#include "ui_SynonymsDialog.h"

#include <QPushButton>
#include <QErrorMessage>
#include <QCompleter>
#include <QStringListModel>
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
#include "ModelView/WordsListModel.h"

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
	saveVocabularySynonyms();
	saveArchiveSynonyms();
}

void SynonymsDialog::initVocabularyCombo()
{
	initVocabularyComboWithFilter();
	//initVocabularyComboWithoutFilter();

	ui->formLayout->replaceWidget( ui->cbVocabularyWords, cmbVocabulary );
	//ui->cbVocabularyWords->hide();
	delete ui->cbVocabularyWords;
}

void SynonymsDialog::initVocabularyComboWithoutFilter()
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
}

void SynonymsDialog::initVocabularyComboWithFilter()
{
	cmbVocabulary	= new MultiSelectComboBox( this );
	cmbVocabulary->setDisplayText( tr( "-- Select Vocabulary Words --" ) );

	VocabularyWordsModel *model	= new VocabularyWordsModel();
	model->qxFetchAll();
	QList<int> synonyms			= getVocabularySynonyms();

	WordsListModel *comboModel		= new WordsListModel( model, synonyms );
	QSortFilterProxyModel *proxy	= new QSortFilterProxyModel;
	proxy->setSourceModel( comboModel );
	//proxy->setFilterCaseSensitivity( Qt::CaseInsensitive );

	cmbVocabulary->setModel( proxy );
	cmbVocabulary->setEditable( true );
	cmbVocabulary->setCompleter( 0 );

	// When the edit text changes, use it to filter the proxy model.
	connect( cmbVocabulary, SIGNAL( editTextChanged( QString ) ), proxy, SLOT( setFilterWildcard( QString ) ) );
}

void SynonymsDialog::initArchiveCombo()
{
	initArchiveComboWithFilter();
	//initArchiveComboWithoutFilter();

	ui->formLayout->replaceWidget( ui->cbArchiveWords, cmbArchive );
	//ui->cbArchiveWords->hide();
	delete ui->cbArchiveWords;
}

void SynonymsDialog::initArchiveComboWithoutFilter()
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
}

void SynonymsDialog::initArchiveComboWithFilter()
{
	cmbArchive	= new MultiSelectComboBox( this );
	cmbArchive->setDisplayText( tr( "-- Select Vocabulary Words --" ) );

	qx::QxModel<ArchiveWord> *model	= new qx::QxModel<ArchiveWord>();
	model->qxFetchAll();
	QList<int> synonyms				= getArchiveSynonyms();

	WordsListModel *comboModel		= new WordsListModel( model, synonyms );
	QSortFilterProxyModel *proxy	= new QSortFilterProxyModel;
	proxy->setSourceModel( comboModel );
	//proxy->setFilterCaseSensitivity( Qt::CaseInsensitive );

	cmbArchive->setModel( proxy );
	cmbArchive->setEditable( true );
	cmbArchive->setCompleter( 0 );

	// When the edit text changes, use it to filter the proxy model.
	connect( cmbArchive, SIGNAL( editTextChanged( QString ) ), proxy, SLOT( setFilterWildcard( QString ) ) );
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
