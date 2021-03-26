#include "Import.h"

#include <QtXml>
#include <QFile>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyMetaInfo.h"
#include "Entity/VocabularyGroup.h"
#include "Entity/Vocabulary.h"

/*
 * Tutorial: https://lucidar.me/en/dev-c-cpp/reading-xml-files-with-qt/
 */
Import::Import()
{

}

Import::~Import()
{

}

VocabularyGroupPtr Import::createGroup( QString groupName )
{
	QString query;
	QSqlError daoError;
	VocabularyGroupPtr vocg;
	qx::QxModel<VocabularyGroup>* pModel	= new qx::QxModel<VocabularyGroup>();

	query		= QString( "WHERE name='%1'" ).arg( groupName );
	pModel->qxFetchByQuery( query );

	int i	= 1;
	QString searchGroupName;
	while( pModel->rowCount() ) {
		pModel			= new qx::QxModel<VocabularyGroup>();
		searchGroupName	= groupName + "_" + QString::number( i );
		query			= QString( "WHERE name='%1'" ).arg( searchGroupName );
		pModel->qxFetchByQuery( query );

		i++;
	}

	if ( !  searchGroupName.isEmpty() )
		groupName	= searchGroupName;

	vocg		= VocabularyGroupPtr( new VocabularyGroup() );
	vocg->name	= groupName;
	daoError	= qx::dao::insert( vocg );

	return vocg;
}

bool Import::importFromFile( QString file )
{
	Q_UNUSED( file );

	return false;
}

bool Import::importFromDb( QString dbName )
{
	Q_UNUSED( dbName );

	return false;
}
