#include "ModelView/VocabularyWordsModel.h"

#include <QMimeData>
#include <QTextCodec>

VocabularyWordsModel::VocabularyWordsModel( QObject* parent ) : QxModel<Vocabulary>( parent )
{

}

VocabularyWordsModel::~VocabularyWordsModel()
{

}

bool VocabularyWordsModel::dropMimeData(const QMimeData *mimeData, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
	    if ( action == Qt::IgnoreAction )
	    	return true;

	    // qDebug() << action;
	    // qDebug() << "MIME TYPE: " << mimeData->formats().last();
	    if ( ! mimeData->hasFormat( "application/x-qabstractitemmodeldatalist" ) )
	        return false;

	    if ( column > 0 )
	    	return false;

	    int destRow;
	    if ( row != -1 )
	        destRow = row;
	    else if ( parent.isValid() )
	        destRow = parent.row();
	    else
	        destRow = rowCount( QModelIndex() );

	    QByteArray encodedData = mimeData->data( mimeData->formats().last() );

		QDataStream stream( &encodedData, QIODevice::ReadOnly );
		QStringList newItems;

		QVector<int> movedRows;
		while ( ! stream.atEnd() ) {
			int row, col;
			QMap<int,  QVariant> roleDataMap;	// data in every index

			stream >> row >> col >> roleDataMap;
			// qDebug() << "MIME DATA: " << QString::number( row ) << ", " << QString::number( col );

			if ( movedRows.isEmpty() || movedRows.last() != row ) {
				qDebug() << "MOUVED ROW: " << row;
				movedRows	<< row;
			}
		}

		if ( movedRows.size() > 0 ) {
			qDebug() << "SOURCE ID: " << data( index( movedRows[0], 0 ) ).toString();
			qDebug() << "DESTINATION ID: " << data( index( destRow, 0 ) ).toString();

			moveRows(
				index( movedRows[0], 0 ),
				movedRows[0],
				movedRows.size(),
				index( destRow, 0 ),
				destRow
			);
		}

	    return true;
}

Qt::DropActions VocabularyWordsModel::supportedDragActions() const
{
	// qDebug() << __FUNCTION__;
	return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions VocabularyWordsModel::supportedDropActions() const
{
	// qDebug() << __FUNCTION__;
	return Qt::CopyAction | Qt::MoveAction;
}

Qt::ItemFlags VocabularyWordsModel::flags( const QModelIndex & index ) const
{
	Qt::ItemFlags defaultFlags	= Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
	if ( index.isValid() )
		return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
	else
		return Qt::ItemIsDropEnabled | defaultFlags;
}

bool VocabularyWordsModel::moveRows(
	const QModelIndex &sourceIndex,
	int sourceRow,
	int countRows,
	const QModelIndex &destinationIndex,
	int destinationRow
) {
	beginMoveRows( sourceIndex, sourceRow, sourceRow + countRows - 1, destinationIndex, destinationRow );

	// Begin Update Rows
	ListVocabulary	movedRows;
	ListVocabulary	updatedRows;
	VocabularyPtr vocbPtr;
	QSqlError daoError;
	int errors = 0;

	// Update Moved Rows
	for ( int row=0; row < countRows; ++row ) {
		vocbPtr.reset( new Vocabulary() );
		int movedId	= data( index( sourceRow, 0 ) ).toInt();
		qDebug() << "Moved ID: " << QString::number( movedId );
		vocbPtr->id	= movedId;
		daoError = qx::dao::fetch_by_id( vocbPtr );
		if ( ! daoError.isValid() ) {
			vocbPtr->id	= data( index( destinationRow, 0 ) ).toInt();
			movedRows << vocbPtr;
		} else {
			errors++;
			qDebug() << "ERROR: " << daoError.text();
		}
	}

	// Update Destination Rows
	qx::QxSqlQuery query	= qx_query();
	query.freeText( "where id >= " + data( index( destinationRow, 0 ) ).toString() );
	daoError = qx::dao::fetch_by_query( query, updatedRows );
	for ( int row = 0; row < updatedRows.size(); ++row ) {
		int destId	= updatedRows.at( row )->id;
		qDebug() << "Updateed Dest ID: " << QString::number( destId );

		updatedRows.at( row )->id	= destId + 1;
	}

	// Update All Rows
	QSqlDatabase db	= qx::QxSqlDatabase::getDatabase();
	bool commit 	= db.transaction();
	daoError = qx::dao::update( updatedRows, &db );
	if ( ! daoError.isValid() ) {
		daoError = qx::dao::update( updatedRows, &db );
	} else {
		qDebug() << "ERROR: " << daoError.text();
		errors ++;
	}

	daoError = qx::dao::update( movedRows, &db );
	if ( commit && errors == 0 && ! daoError.isValid() )
		db.commit();
	else
		db.rollback();

	// End Update Rows
	endMoveRows();

	return true;
}
