#include "MicrosoftVocabulary.h"

#include <QtXml>
#include <QFile>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Application/VsDatabase.h"
#include "Entity/VocabularyMetaInfo.h"
#include "Entity/VocabularyGroup.h"
#include "Entity/Vocabulary.h"

QDomDocument* MicrosoftVocabulary::xml;

bool MicrosoftVocabulary::importFromFile( QString file )
{
	xml	= new QDomDocument();

	// Load xml file as raw data
	QFile f( file );
	if ( ! f.open(QIODevice::ReadOnly ) )
	{
		// Error while loading file
		std::cerr << "Error while loading file" << std::endl;
		return 1;
	}
	// Set data into the QDomDocument before processing
	xml->setContent( &f );
	f.close();

	//parseMeta();
	parseWords();
}

bool MicrosoftVocabulary::exportToFile( QString file )
{
	QDomDocument doc( "Dictionary" );
	doc.appendChild( doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"utf-8\"" ) );

	QDomElement root = doc.createElement( "Dictionary" );
	root.setAttribute( "xmlns:xsd", "http://www.w3.org/2001/XMLSchema" );
	root.setAttribute( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" );
	doc.appendChild( root );

	// Export Meta Info
	VocabularyMetaInfoPtr metaInfo	= VsDatabase::instance()->metaInfo();
	QDomElement elMetaLanguage0	= doc.createElement( "Language0" );
	QDomElement elMetaLanguage1	= doc.createElement( "Language1" );
	QDomText txtMetaLanguage0	= doc.createTextNode( metaInfo->language1 );
	QDomText txtMetaLanguage1	= doc.createTextNode( metaInfo->language2 );
	elMetaLanguage0.appendChild( txtMetaLanguage0 );
	elMetaLanguage1.appendChild( txtMetaLanguage1 );
	root.appendChild( elMetaLanguage0 );
	root.appendChild( elMetaLanguage1 );

	// Export WordList
	QDomElement word;
	QDomElement elLanguage0;
	QDomElement elLanguage1;
	QDomText txtLanguage0;
	QDomText txtLanguage1;
	QDomElement wordlist			= doc.createElement( "Wordlist" );
	qx::QxModel<Vocabulary>* model	= new qx::QxModel<Vocabulary>();

	model->qxFetchAll();
	for( int r = 0; r < model->rowCount(); ++r ) {
		word			= doc.createElement( "Word" );
		elLanguage0		= doc.createElement( "Language0" );
		elLanguage1		= doc.createElement( "Language1" );

		txtLanguage0 	= doc.createTextNode( model->data( model->index( r, 1 ) ).toString() );
		txtLanguage1 	= doc.createTextNode( model->data( model->index( r, 2 ) ).toString() );

		elLanguage0.appendChild( txtLanguage0 );
		elLanguage1.appendChild( txtLanguage1 );
		word.appendChild( elLanguage0 );
		word.appendChild( elLanguage1 );
		wordlist.appendChild( word );
	}
	root.appendChild( wordlist );

	// Write to File
	QFile f( file );
	if ( ! f.open( QIODevice::WriteOnly | QIODevice::Text ) )
		return false;

	QString xml = doc.toString();
	QTextStream out( &f );
	out << xml;

	return true;
}

bool MicrosoftVocabulary::parseMeta()
{
	// MetaInfo Languages
	QString language1	= xml->documentElement().firstChild().toElement().text();
	QString language2	= xml->documentElement().firstChild().nextSibling().toElement().text();

	// Insert VocabularyMetaInfo
//	VocabularyMetaInfoPtr metaInfo;
//	metaInfo.reset( new VocabularyMetaInfo() );
//	metaInfo->name			= ui->leName->text();
//	metaInfo->language1		= ui->leLanguage_1->text();
//	metaInfo->language2		= ui->leLanguage_2->text();
//
//	QSqlError daoError	= qx::dao::insert( metaInfo );

	return true;
}

bool MicrosoftVocabulary::parseWords()
{
	QSqlError daoError;
	VocabularyPtr voc;
	VocabularyGroupPtr vocg	= createGroup();

	QDomElement word	= xml->elementsByTagName( "Wordlist" ).at( 0 ).firstChild().toElement();
	while( ! word.isNull() )
	{
		voc				= VocabularyPtr( new Vocabulary() );

		voc->group_id		= vocg->id;	// To can Move Groups( I dont know how to do this with related object)
		//voc->group		= vocg;

		voc->language_1	= word.firstChild().toElement().text();
		voc->language_2	= word.firstChild().nextSibling().toElement().text();
		daoError		= qx::dao::insert( voc );

		word			= word.nextSibling().toElement();
	}

	return true;
}
