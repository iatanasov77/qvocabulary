#include "VsComboBoxModel.h"

#include <QDebug>

VsComboBoxModel::VsComboBoxModel( QAbstractItemModel *sourceModel, QList<int> selectedItems, QObject *parent )
{
	Q_UNUSED( parent );

	source		= sourceModel;
	selected	= selectedItems;

	populate();
}

int VsComboBoxModel::rowCount( const QModelIndex &parent ) const
{
	Q_UNUSED( parent );

	return source->rowCount();
}

QVariant VsComboBoxModel::data( const QModelIndex &index, int role ) const
{
	if ( ! index.isValid() )
		return QVariant();

	int id			= source->data( source->index( index.row(), 0 ) ).toInt();
	QString word	= source->data( source->index( index.row(), 1 ) ).toString();
	switch ( role ) {
		case Qt::CheckStateRole:
			return selected.contains( id ); // Set Check State
			break;
		case Qt::UserRole:
			return id;	// return Word ID
			break;
		case Qt::DisplayRole:
			return word; // return Word Text
			break;
		default:
			return QVariant();
	}
}

bool VsComboBoxModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
	if ( role == Qt::CheckStateRole ) {
		int id	= source->data( source->index( index.row(), 0 ) ).toInt();
		if ( selected.contains( id ) ) {
			selected.removeOne( id );
		} else {
			selected.append( id );
		}

		emit dataChanged( index, index, QVector<int>( role ) );
		return true;
	}

	return QAbstractListModel::setData( index, value, role );
}

void VsComboBoxModel::populate()
{
//	beginResetModel();
//	mDatas.clear();
//	mDatas.append( User("Charles", "Darwin", QDate(1812,22,23)));
//	mDatas.append(User("Lars", "Knoll", QDate(1976,22,12)));
//	mDatas.append(User("Boby", "Lapointe", QDate(1951,21,31)));
//	endResetModel();
}
