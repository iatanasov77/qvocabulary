#include "ModelView/SettingsMenu/TreeModel.h"
#include "ModelView/SettingsMenu/TreeItem.h"

#include <QStringList>
#include <QDomDocument>
#include <QApplication>

TreeModel::TreeModel( const QString &data, QObject *parent )
    : QAbstractItemModel( parent )
{
	rootItem = new TreeItem( { tr( "Title" ), tr( "Id" ) } );
	setupModelData( data, rootItem );
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

QModelIndex TreeModel::index( int row, int column, const QModelIndex &parent ) const
{
    if ( ! hasIndex( row, column, parent ) )
        return QModelIndex();

    TreeItem *parentItem;

    if ( ! parent.isValid() )
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>( parent.internalPointer() );

    TreeItem *childItem = parentItem->child( row );
    if ( childItem )
        return createIndex( row, column, childItem );
    return QModelIndex();
}

QModelIndex TreeModel::parent( const QModelIndex &index ) const
{
    if ( ! index.isValid() )
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>( index.internalPointer() );
    TreeItem *parentItem = childItem->parentItem();

    if ( parentItem == rootItem )
        return QModelIndex();

    return createIndex( parentItem->row(), 0, parentItem );
}

int TreeModel::rowCount( const QModelIndex &parent ) const
{
    TreeItem *parentItem;
    if ( parent.column() > 0 )
        return 0;

    if ( ! parent.isValid() )
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>( parent.internalPointer() );

    return parentItem->childCount();
}

int TreeModel::columnCount( const QModelIndex &parent ) const
{
    if ( parent.isValid() )
        return static_cast<TreeItem*>( parent.internalPointer() )->columnCount();
    return rootItem->columnCount();
}

QVariant TreeModel::data( const QModelIndex &index, int role ) const
{
    if ( ! index.isValid() )
        return QVariant();

    if ( role != Qt::DisplayRole )
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>( index.internalPointer() );

    return index.column() == 0 ?
    		QVariant( qApp->translate( "SettingsMenu", item->data( index.column() ).toString().toStdString().c_str() ) ) :
    		item->data( index.column() );
}

Qt::ItemFlags TreeModel::flags( const QModelIndex &index ) const
{
    if ( ! index.isValid() )
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags( index );
}

QVariant TreeModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
        return rootItem->data( section );

    return QVariant();
}

void TreeModel::setupModelData( const QString data, TreeItem *parent )
{
	QDomDocument* xml	= new QDomDocument();
	xml->setContent( data );

	// First Child
	QDomElement item	= xml->elementsByTagName( "MenuItems" ).at( 0 ).firstChild().toElement();
	iterateChildItems( parent, item );
}

void TreeModel::iterateChildItems( TreeItem* parent, QDomElement item )	// item passed is first child of the parent
{
	while( ! item.isNull() )
	{
		QString id		= item.attribute( "id" );
		// Title translations are in `GlobalTranslations.h`
		QString title	= item.elementsByTagName( "Title" ).at( 0 ).toElement().text();

		QVector<QVariant> columnData;
		columnData << title << id;

		parent->appendChild( new TreeItem( columnData, parent ) );

		QDomNode	firstChild	= item.elementsByTagName( "MenuItems" ).at( 0 ).firstChild();
		if ( ! firstChild.isNull() ) {	// Has Childs
			iterateChildItems( parent->child( parent->childCount() -1 ), firstChild.toElement() );
		}

		item	= item.nextSibling().toElement();
	}
}
