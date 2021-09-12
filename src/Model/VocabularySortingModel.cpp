#include "VocabularySortingModel.h"

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "GlobalTypes.h"

VocabularySortingModel::VocabularySortingModel( QObject *parent )
    : QSortFilterProxyModel(parent)
{
	setSortCaseSensitivity( Qt::CaseInsensitive );
	clearFilter();
}

void VocabularySortingModel::clearFilter()
{
	wordType	= -1;
	invalidateFilter();
}

void VocabularySortingModel::clearSorting()
{
	sort( 0, Qt::AscendingOrder );
	invalidate();
}

void VocabularySortingModel::setFilterWordType( int type )
{
	wordType	= type;
	invalidateFilter();
}

/**
 * Exclude Empty Line From Sorting
 * Manual: https://stackoverflow.com/questions/29350352/qabstracttablemodel-empty-last-row-without-sorting
 */
bool VocabularySortingModel::lessThan( const QModelIndex &left, const QModelIndex &right ) const
{
	bool leftId		= sourceModel()->data( left.siblingAtColumn( 0 ) ).toInt();
	bool rightId	= sourceModel()->data( right.siblingAtColumn( 0 ) ).toInt();

    if ( leftId == 0 )
        return ( this->sortOrder() == Qt::DescendingOrder );
    else if ( rightId == 0 )
        return ( this->sortOrder() == Qt::AscendingOrder );
    else {
    	//return leftId < rightId;
    	return QSortFilterProxyModel::lessThan( left, right );
    }

}

bool VocabularySortingModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
	Q_UNUSED( sourceParent )

    int wordId	= sourceModel()->data( sourceModel()->index( sourceRow, 0 ) ).toInt();

    return  wordType >= 0 ? getTranslationsTypes( wordId ).contains( wordType ) : true;
}

QList<int> VocabularySortingModel::getTranslationsTypes( int wordId ) const
{
	QSqlQuery query		= QSqlQuery( qx::QxSqlDatabase::getDatabase() );
	QString strQuery	= QString( "SELECT tr_type FROM VocabularyWordTranslation "
									"WHERE word_id=%1" )
								.arg( wordId );
	query.exec( strQuery );

	QList<int> types;
	while ( query.next() ) {
		types << TranslationTypesList.indexOf( query.value( "tr_type" ).toString() );
	}

	return types;
}
