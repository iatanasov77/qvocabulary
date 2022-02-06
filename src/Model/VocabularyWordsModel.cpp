#include "VocabularyWordsModel.h"

#include <QMimeData>
#include <QTextCodec>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "GlobalTypes.h"
#include "Entity/Vocabulary.h"
#include "Application/VsDatabase.h"

VocabularyWordsModel::VocabularyWordsModel( QObject* parent ) : QxModel<Vocabulary>( parent )
{
	// QxModelView module : new feature available to add automatically an empty row at the end of the table to insert quickly new items (setShowEmptyLine() method)
	setShowEmptyLine( true );
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

	return true;
}

Qt::DropActions VocabularyWordsModel::supportedDragActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions VocabularyWordsModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}

Qt::ItemFlags VocabularyWordsModel::flags( const QModelIndex & index ) const
{
	Qt::ItemFlags defaultFlags;
	if ( index.column() == 6 ) {
		defaultFlags	= Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	} else {
		defaultFlags	= Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
	}

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
	QSqlQuery *origTableQuery 	= new QSqlQuery( qx::QxSqlDatabase::getDatabase() );
	QSqlQuery *tempTableQuery 	= new QSqlQuery( qx::QxSqlDatabase::getDatabase() );

	QVector<int> srcIds;
	for ( int i = 0; i < countRows; i++ ) {
		srcIds	<< data( index( sourceRow + i, 0 ) ).toInt();
	}
	_prepareMove();

	int id;
	int destId	= _getDestinationId( destinationRow );

	origTableQuery->exec( "SELECT * FROM Vocabulary" );
	while ( origTableQuery->next() ) {
		id	= origTableQuery->value( "id" ).toInt();

		if ( srcIds.contains( id ) ) {
			id	= destId + srcIds.indexOf( id );
		} else if ( id >= destId ) {
			id	= id + countRows;
		}

		_tempQueryInsert( id, origTableQuery, tempTableQuery );
	}

	_finishMove();
}

int VocabularyWordsModel::_getDestinationId( int destinationRow )
{
	int destId	= data( index( destinationRow, 0 ) ).toInt();
	if ( ! destId ) {
		// Drop Placed On DirtyRow
		destId	= data( index( rowCount() - 2, 0 ) ).toInt() + 1;
	}

	return destId;
}

void VocabularyWordsModel::_tempQueryInsert( int id, QSqlQuery *origTableQuery, QSqlQuery *tempTableQuery )
{
	QString queryInsert	= QString(
		"INSERT INTO TempVocabulary( id, language_1, transcription, language_2, group_id, description ) "
		"VALUES( %1, '%2', '%3', '%4', %5, '%6' )"
	)
	.arg( id )
	.arg( origTableQuery->value( "language_1" ).toString() )
	.arg( origTableQuery->value( "transcription" ).toString() )
	.arg( origTableQuery->value( "language_2" ).toString() )
	.arg( origTableQuery->value( "group_id" ).toString() )
	.arg( origTableQuery->value( "description" ).toString() );

	tempTableQuery->exec( queryInsert );
}

void VocabularyWordsModel::_prepareMove()
{
	QSqlQuery *query 		= new QSqlQuery( qx::QxSqlDatabase::getDatabase() );
	QString tempTableSql	= VsDatabase::instance()->getTableCreateSql( "Vocabulary" ).replace( "Vocabulary", "TempVocabulary" );

	query->exec( "DROP TABLE IF EXISTS TempVocabulary" );
	query->exec( tempTableSql );
}

void VocabularyWordsModel::_finishMove()
{
	QSqlQuery *query 			= new QSqlQuery( qx::QxSqlDatabase::getDatabase() );
	QString vocabularyTableSql	= VsDatabase::instance()->getTableCreateSql( "Vocabulary" );

	query->exec( "DROP TABLE IF EXISTS Vocabulary" );
	query->exec( vocabularyTableSql );

	QString queryInsert	= QString(
		"INSERT INTO Vocabulary( id, language_1, transcription, language_2, group_id, description ) "
		"SELECT id, language_1, transcription, language_2, group_id, description FROM TempVocabulary"
	);
	query->exec( queryInsert );
}

/**
 * Added for Synonyms
 */
QVariant VocabularyWordsModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( section == 6 && orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
		return tr( "Synonyms" );
	} else {
		return QxModel<Vocabulary>::headerData( section, orientation, role );
	}
}

QVariant VocabularyWordsModel::data( const QModelIndex &index, int role ) const
{
	if ( index.column() == 6 ) {
		QString synonyms;
		QMap<QString, QVariant> userData;

		QMap<QString, QVariant> vocabularySynonyms	= getVocabularySynonyms( index );
		QMap<QString, QVariant> archiveSynonyms		= getArchiveSynonyms( index );
		QString onlyWordsSynonyms					= getOnlyWordsSynonyms( index );

		//qDebug() << "INIT WORD IDS: " << synonymIds;
		if ( vocabularySynonyms["words"].toString().length() ) {
			synonyms	= vocabularySynonyms["words"].toString();
			userData.insert( SynonymTargets["VOCABULARY"], vocabularySynonyms["ids"] );
		}
		//qDebug() << "VOCABULARY WORD IDS: " << synonymIds;
		if ( archiveSynonyms["words"].toString().length() ) {
			synonyms.append( QString ( ", %1" ).arg( archiveSynonyms["words"].toString() ) );
			userData.insert( SynonymTargets["ARCHIVE"], archiveSynonyms["ids"] );
		}
		//qDebug() << "ARCHIVE WORD IDS: " << synonymIds;
		if ( onlyWordsSynonyms.length() ) {
			synonyms.append( QString ( ", %1" ).arg( onlyWordsSynonyms ) );
			userData.insert( SynonymTargets["ONLY_WORDS"], QList<QVariant>() );
		}

		if ( role == Qt::DisplayRole ) {
			return synonyms;
		} else if ( role == Qt::UserRole ) {
			return userData;
		}
	}

	return QxModel<Vocabulary>::data( index, role );
}

QMap<QString, QVariant> VocabularyWordsModel::getVocabularySynonyms( const QModelIndex &index ) const
{
	QSqlDatabase db		= qx::QxSqlDatabase::getDatabase();
	QSqlQuery *query	= new QSqlQuery( db );
	QString synonyms;
	QList<QVariant> synonymIds;

	QString strQuery	= QString(
			"SELECT v.id AS wordId, v.language_1 AS synonym "
			"FROM VocabularyWordSynonym s "
			"LEFT JOIN Vocabulary v ON v.id = s.synonym_id "
			"WHERE s.word_id = %1 AND s.target = '%2'"
	)
	.arg( index.siblingAtColumn( 0 ).data().toInt() )
	.arg( SynonymTargets["VOCABULARY"] );

	query->exec( strQuery );
	int i	= 0;
	while ( query->next() ) {
		i++;
		synonymIds << query->value( "wordId" );
		synonyms.append( query->value( "synonym" ).toString() );

		if ( i < query->size() || query->size() == -1 ) {
			synonyms.append( ", " );
		}
	}

	QMap<QString, QVariant> result;
	result["ids"]	= QVariant( synonymIds );
	result["words"]	= QVariant( synonyms );

	return result;
}

QMap<QString, QVariant> VocabularyWordsModel::getArchiveSynonyms( const QModelIndex &index ) const
{
	QSqlDatabase db		= qx::QxSqlDatabase::getDatabase();
	QSqlQuery *query	= new QSqlQuery( db );
	QString synonyms;
	QList<QVariant> synonymIds;

	QString strQuery	= QString(
			"SELECT a.id AS wordId, a.language_1 AS synonym "
			"FROM VocabularyWordSynonym s "
			"LEFT JOIN ArchiveWord a ON a.id = s.synonym_id "
			"WHERE s.word_id = %1 AND s.target = '%2'"
	)
	.arg( index.siblingAtColumn( 0 ).data().toInt() )
	.arg( SynonymTargets["ARCHIVE"] );

	query->exec( strQuery );
	int i	= 0;
	while ( query->next() ) {
		i++;
		synonymIds << query->value( "wordId" );
		synonyms.append( query->value( "synonym" ).toString() );

		if ( i < query->size() || query->size() == -1 ) {
			synonyms.append( ", " );
		}
	}

	QMap<QString, QVariant> result;
	result["ids"]	= QVariant( synonymIds );
	result["words"]	= QVariant( synonyms );

	return result;
}

QString VocabularyWordsModel::getOnlyWordsSynonyms( const QModelIndex &index ) const
{
	QSqlDatabase db		= qx::QxSqlDatabase::getDatabase();
	QSqlQuery *query	= new QSqlQuery( db );

	QString strQuery	= QString(
			"SELECT s.only_words AS synonyms "
			"FROM VocabularyWordSynonym s "
			"WHERE s.word_id = %1 AND target = '%2'"
	)
	.arg( index.siblingAtColumn( 0 ).data().toInt() )
	.arg( SynonymTargets["ONLY_WORDS"] );

	query->exec( strQuery );
	if ( query->next() ) {
		return query->value( "synonyms" ).toString();
	}

	return "";
}
