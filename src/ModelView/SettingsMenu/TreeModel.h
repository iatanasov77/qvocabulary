#ifndef SETTINGSMENU_TREEMODEL_H
#define SETTINGSMENU_TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeItem;

#include "GlobalTypes.h"

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

	public:
		explicit TreeModel( const QString &data, QObject *parent = nullptr, ItemDataSource dataType = Txt );
		~TreeModel();

		QVariant data( const QModelIndex &index, int role ) const override;
		Qt::ItemFlags flags( const QModelIndex &index ) const override;
		QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;
		QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;
		QModelIndex parent( const QModelIndex &index ) const override;
		int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
		int columnCount( const QModelIndex &parent = QModelIndex() ) const override;

		QStringList ids();

	private:
		void setupModelData( const QStringList &lines, TreeItem *parent );
		void setupModelDataFromXml( const QString data, TreeItem *parent );

		TreeItem *rootItem;
		QStringList _ids;
};

#endif	// SETTINGSMENU_TREEMODEL_H
