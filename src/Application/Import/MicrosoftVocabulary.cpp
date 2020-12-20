#include "MicrosoftVocabulary.h"

#include <QtXml>
#include <QFile>

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
