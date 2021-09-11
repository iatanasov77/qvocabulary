#include "VocabularySortingModel.h"

VocabularySortingModel::VocabularySortingModel( QObject *parent )
    : QSortFilterProxyModel(parent)
{
}

/**
 * Exclude Empty Line From Sorting
 * Manual: https://stackoverflow.com/questions/29350352/qabstracttablemodel-empty-last-row-without-sorting
 */
bool VocabularySortingModel::lessThan( const QModelIndex &left, const QModelIndex &right ) const
{
	bool hasLeftId	= sourceModel()->data( left.siblingAtColumn( 0 ) ).toBool();
	bool hasRightId	= sourceModel()->data( left.siblingAtColumn( 0 ) ).toBool();

    if ( ! hasLeftId )
        return ( this->sortOrder() == Qt::DescendingOrder );
    else if ( ! hasRightId )
        return ( this->sortOrder() == Qt::AscendingOrder );
    else
        return QSortFilterProxyModel::lessThan( left, right );
}
