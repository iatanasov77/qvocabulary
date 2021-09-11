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
#include "Model/VocabularyWordsModel.h"
#include "../lib/VankoSoft/Model/VsComboBoxModel.h"

SynonymsDialog::SynonymsDialog( QMap<QString, QVariant> word, QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::SynonymsDialog )
{
    ui->setupUi( this );

    vww = parent;	// parent VocabularyWordsWidget
	_wordId	= word["id"].toInt();
	ui->lblDescription->setText( QString( tr( "Synonyms for word" ) + ": `%1`" ).arg( word["text"].toString() ) );

    initVocabularyCombo();
    initArchiveCombo();
    ui->leOnlyWords->setText( getOnlyWordsSynonyms()["text"].toString() );

    QPushButton *saveButton = ui->buttonBox->button( QDialogButtonBox::Save );
    saveButton->setText( tr( "Save" ) );
    connect( saveButton, SIGNAL( clicked() ), this, SLOT( saveSynonyms() ) );

    // May be there is other way to translate Core Strings
    ui->buttonBox->button( QDialogButtonBox::Cancel )->setText( tr( "Cancel" ) );
}

SynonymsDialog::~SynonymsDialog()
{
    delete ui;
}

void SynonymsDialog::saveSynonyms()
{
	saveVocabularySynonyms();
	saveArchiveSynonyms();
	saveOnlyWordsSynonyms();
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
	cmbVocabulary	= new VsMultiSelectComboBox( this );
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
	cmbVocabulary	= new VsMultiSelectComboBox( this );
	cmbVocabulary->setDisplayText( tr( "-- Select Vocabulary Words --" ) );

	VocabularyWordsModel *model	= new VocabularyWordsModel();
	model->qxFetchAll();
	QList<int> synonyms			= getVocabularySynonyms();

	VsComboBoxModel *comboModel		= new VsComboBoxModel( model, synonyms );
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
	cmbArchive	= new VsMultiSelectComboBox( this );
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
	cmbArchive	= new VsMultiSelectComboBox( this );
	cmbArchive->setDisplayText( tr( "-- Select Archive Words --" ) );

	qx::QxModel<ArchiveWord> *model	= new qx::QxModel<ArchiveWord>();
	model->qxFetchAll();
	QList<int> synonyms				= getArchiveSynonyms();

	VsComboBoxModel *comboModel		= new VsComboBoxModel( model, synonyms );
	QSortFilterProxyModel *proxy	= new QSortFilterProxyModel;
	proxy->setSourceModel( comboModel );
	//proxy->setFilterCaseSensitivity( Qt::CaseInsensitive );

	cmbArchive->setModel( proxy );
	cmbArchive->setEditable( true );
	cmbArchive->setCompleter( 0 );

	// When the edit text changes, use it to filter the proxy model.
	connect( cmbArchive, SIGNAL( editTextChanged( QString ) ), proxy, SLOT( setFilterWildcard( QString ) ) );
}

QList<int> SynonymsDialog::selectedVocabularyWords()
{
	QList<int> words;
	for ( int i = 0; i < cmbVocabulary->count(); i++ ) {
		if ( cmbVocabulary->itemData( i, Qt::CheckStateRole ).toBool() ) {
			//QString word	= cmbVocabulary->itemData( i, Qt::DisplayRole ).toString();
			int wordId	= cmbVocabulary->itemData( i, Qt::UserRole ).toInt();
			words << wordId;
		}
	}

	return words;
}

QList<int> SynonymsDialog::selectedArchiveWords()
{
	QList<int> words;
	for ( int i = 0; i < cmbArchive->count(); i++ ) {
		if ( cmbArchive->itemData( i, Qt::CheckStateRole ).toBool() ) {
			//QString word	= cmbVocabulary->itemData( i, Qt::DisplayRole ).toString();
			int wordId	= cmbArchive->itemData( i, Qt::UserRole ).toInt();
			words << wordId;
		}
	}

	return words;
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

QMap<QString, QVariant> SynonymsDialog::getOnlyWordsSynonyms()
{
	QMap<QString, QVariant> onlyWords;

	QSqlQuery query(
		QString( "SELECT * FROM VocabularyWordSynonym WHERE word_id=%1 AND target = '%2'" )
			.arg( _wordId )
			.arg( SynonymTargets["ONLY_WORDS"] )
		,  qx::QxSqlDatabase::getDatabase()
	);

	if ( query.next() ) {
		onlyWords["id"]		= query.value( "id" ).toInt();
		onlyWords["text"]	= query.value( "only_words" ).toString();
	} else {
		onlyWords["id"]		= 0;
		onlyWords["text"]	= "";
	}

	return onlyWords;
}

void SynonymsDialog::saveVocabularySynonyms()
{
	QString strQuery		= "";
	QSqlQuery query			= QSqlQuery( qx::QxSqlDatabase::getDatabase() );
	QList<int> newSynonyms	= selectedVocabularyWords();
	QList<int> oldSynonyms	= getVocabularySynonyms();

	foreach ( int synonymId, oldSynonyms ) {
		if ( ! newSynonyms.contains( synonymId ) ) {
			strQuery += QString( "DELETE FROM VocabularyWordSynonym WHERE word_id=%1 AND synonym_id=%2 AND target = '%3';" )
							.arg( _wordId )
							.arg( synonymId )
							.arg( SynonymTargets["VOCABULARY"] );

			strQuery += QString( "DELETE FROM VocabularyWordSynonym WHERE word_id=%1 AND synonym_id=%2 AND target = '%3';" )
							.arg( synonymId )
							.arg( _wordId )
							.arg( SynonymTargets["VOCABULARY"] );
		}
	}

	foreach ( int synonymId, newSynonyms ) {
		if ( oldSynonyms.contains( synonymId ) )
			continue;

		strQuery += QString( "INSERT INTO VocabularyWordSynonym( word_id, synonym_id, target ) VALUES( %1, %2, '%3' );" )
						.arg( _wordId )
						.arg( synonymId )
						.arg( SynonymTargets["VOCABULARY"] );

		strQuery += QString( "INSERT INTO VocabularyWordSynonym( word_id, synonym_id, target ) VALUES( %1, %2, '%3' );" )
						.arg( synonymId )
						.arg( _wordId )
						.arg( SynonymTargets["VOCABULARY"] );

	}

	if ( strQuery.length() ) {
		QStringList scriptQueries = strQuery.split( ';' );
		foreach ( QString queryTxt, scriptQueries ) {
			if ( queryTxt.trimmed().isEmpty() ) {
				continue;
			}

			if ( ! query.exec( queryTxt ) ) {
				qDebug() << query.lastError().text();
			}
			query.finish();
		}
	}
}

void SynonymsDialog::saveArchiveSynonyms()
{
	QString strQuery		= "";
	QSqlQuery query			= QSqlQuery( qx::QxSqlDatabase::getDatabase() );
	QList<int> newSynonyms	= selectedArchiveWords();
	QList<int> oldSynonyms	= getArchiveSynonyms();

	foreach ( int synonymId, oldSynonyms ) {
		if ( ! newSynonyms.contains( synonymId ) ) {
			strQuery += QString( "DELETE FROM VocabularyWordSynonym WHERE word_id=%1 AND synonym_id=%2 AND target='%3';" )
							.arg( _wordId )
							.arg( synonymId )
							.arg( SynonymTargets["ARCHIVE"] );

//	Revert query for Archive Synonyms is Not Needed and Not Well
//	==============================================================
//			strQuery += QString( "DELETE FROM VocabularyWordSynonym WHERE word_id=%1 AND synonym_id=%2 AND target = '%3';" )
//							.arg( synonymId )
//							.arg( _wordId )
//							.arg( SynonymTargets["ARCHIVE"] );
		}
	}

	foreach ( int synonymId, newSynonyms ) {
		if ( oldSynonyms.contains( synonymId ) )
			continue;

		strQuery += QString( "INSERT INTO VocabularyWordSynonym( word_id, synonym_id, target ) VALUES( %1, %2, '%3' );" )
						.arg( _wordId )
						.arg( synonymId )
						.arg( SynonymTargets["ARCHIVE"] );

//	Revert query for Archive Synonyms is Not Needed and Not Well
//	===============================================================
//		strQuery += QString( "INSERT INTO VocabularyWordSynonym( word_id, synonym_id, target ) VALUES( %1, %2, '%3' );" )
//						.arg( synonymId )
//						.arg( _wordId )
//						.arg( SynonymTargets["ARCHIVE"] );
	}

	if ( strQuery.length() ) {
		QStringList scriptQueries = strQuery.split( ';' );
		foreach ( QString queryTxt, scriptQueries ) {
			if ( queryTxt.trimmed().isEmpty() ) {
				continue;
			}

			if ( ! query.exec( queryTxt ) ) {
				qDebug() << query.lastError().text();
			}
			query.finish();
		}
	}
}

void SynonymsDialog::saveOnlyWordsSynonyms()
{
	QString strQuery;
	int existId	= getOnlyWordsSynonyms()["id"].toInt();
	if ( ui->leOnlyWords->text().length() ) {
		if ( existId ) {
			strQuery	= QString( "UPDATE VocabularyWordSynonym SET only_words='%1' WHERE id=%2" )
							.arg( ui->leOnlyWords->text() )
							.arg( QString::number( existId ) );
		} else {
			strQuery	= QString( "INSERT INTO VocabularyWordSynonym( word_id, only_words, target ) VALUES( %1, '%2', '%3' )" )
							.arg( _wordId )
							.arg( ui->leOnlyWords->text() )
							.arg( SynonymTargets["ONLY_WORDS"] );
		}

		QSqlQuery query( strQuery, qx::QxSqlDatabase::getDatabase() );
	}

	if ( ! ui->leOnlyWords->text().length() && existId ) {
		strQuery = QString( "DELETE FROM VocabularyWordSynonym WHERE id=%1;" ).arg( existId );
		QSqlQuery query( strQuery, qx::QxSqlDatabase::getDatabase() );
	}
}
