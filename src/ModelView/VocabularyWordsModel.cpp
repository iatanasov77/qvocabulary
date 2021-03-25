#include "ModelView/VocabularyWordsModel.h"

#include <QMimeData>
#include <QTextCodec>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"
#include "Entity/Vocabulary.h"

VocabularyWordsModel::VocabularyWordsModel( QObject* parent ) : QxModel<Vocabulary>( parent )
{

}

VocabularyWordsModel::~VocabularyWordsModel()
{

}

bool VocabularyWordsModel::dropMimeData(const QMimeData *mimeData, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
	    if ( action == Qt::IgnoreAction || ! mimeData->hasFormat( "application/x-qabstractitemmodeldatalist" ) || column > 0 )
	    	return true;

	    int destRow;
	    if ( row != -1 ) {
	        destRow = row;
	    } else if ( parent.isValid() ) {
	        destRow = parent.row();
	    } else {
	        destRow = rowCount( QModelIndex() );
	    }

	    QByteArray encodedData = mimeData->data( mimeData->formats().last() );

		QDataStream stream( &encodedData, QIODevice::ReadOnly );
		QStringList newItems;
		QVector<int> movedRows;
		while ( ! stream.atEnd() ) {
			int row, col;
			QMap<int,  QVariant> roleDataMap;	// data in every index
			stream >> row >> col >> roleDataMap;

			if ( movedRows.isEmpty() || movedRows.last() != row ) {
				movedRows	<< row;
			}
		}

		if ( movedRows.size() > 0 ) {
			moveRows(
				index( movedRows[0], 0 ),
				movedRows[0],
				movedRows.size(),
				index( destRow, 0 ),
				destRow
			);
		}

		//qDebug() << "Count Moved Rows: " << QString::number( movedRows.size() );
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
	Q_UNUSED( sourceIndex );
	Q_UNUSED( destinationIndex );

	QSqlError daoError;

	beginMoveRows( QModelIndex(), sourceRow, sourceRow + countRows - 1, QModelIndex(), destinationRow );
	QSqlDatabase db				= qx::QxSqlDatabase::getDatabase();
	bool commit 				= db.transaction();

	_myMoveRows( sourceRow, destinationRow, countRows );

	if ( ! commit ) {
		db.rollback();
		return false;
	}

	db.commit();
	endMoveRows();
	emit modelUpdated();

	return true;
}

void VocabularyWordsModel::_myMoveRows( int sourceRow, int destinationRow, int countRows )
{
	int id;
	QVector<int> srcIds;
	QString queryInsert;
	QSqlDatabase db				= qx::QxSqlDatabase::getDatabase();
	QSqlQuery *query 			= new QSqlQuery( db );
	QSqlQuery *tempTableQuery	= new QSqlQuery( db );

	for ( int i = 0; i < countRows; i++ ) {
		srcIds	<< data( index( sourceRow + i, 0 ) ).toInt();
	}
	int destId					= data( index( destinationRow, 0 ) ).toInt();

	query->exec( "DROP TABLE IF EXISTS TempVocabulary" );
	query->exec( "CREATE TABLE TempVocabulary( id integer primary key, language_1 varchar(255), transcription varchar(255), language_2 varchar(255), group_id integer )" );

	query->exec( "SELECT * FROM Vocabulary" );
	while ( query->next() ) {
		id	= query->value( "id" ).toInt();

		if ( srcIds.contains( id ) ) {
			id	= destId + srcIds.indexOf( id );
		} else if ( id >= destId ) {
			id	= id + countRows;
		}

		queryInsert	= "INSERT INTO TempVocabulary( id, language_1, transcription, language_2, group_id ) VALUES( " + QString::number( id ) + ", '" + query->value( "language_1" ).toString() + "', '" + query->value( "transcription" ).toString() + "', '" + query->value( "language_2" ).toString() + "', " + query->value( "group_id" ).toString() + " )";
		tempTableQuery->exec( queryInsert );
	}

	query->exec( "DROP TABLE IF EXISTS Vocabulary" );
	query->exec( "CREATE TABLE Vocabulary( id integer primary key, language_1 varchar(255), transcription varchar(255), language_2 varchar(255), group_id integer )" );
	query->exec( "INSERT INTO Vocabulary( id, language_1, transcription, language_2, group_id ) SELECT id, language_1, transcription, language_2, group_id FROM TempVocabulary" );
}
