#ifndef MODEL_VOCABULARYWORDS_H
#define MODEL_VOCABULARYWORDS_H

#include <QObject>
#include <QMap>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/Vocabulary.h"

class VocabularyWordsModel : public qx::QxModel<Vocabulary>
{
	Q_OBJECT

	public:
		VocabularyWordsModel( QObject* parent = nullptr );
	    virtual ~VocabularyWordsModel();

	    virtual Qt::DropActions supportedDragActions() const;
	    virtual Qt::DropActions supportedDropActions() const;
		virtual Qt::ItemFlags flags( const QModelIndex & index ) const;

		virtual bool dropMimeData(
			const QMimeData *data,
			Qt::DropAction action,
			int row,
			int column,
			const QModelIndex &parent
		);

		virtual bool moveRows(
			const QModelIndex &sourceIndex,
			int sourceRow,
			int countRows,
			const QModelIndex &destinationIndex,
			int destinationRow
		);

		QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;	// Added for Synonyms
		QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;						// Added for Synonyms

	private:
		void _myMoveRows( int sourceRow, int destinationRow, int countRows );
		QMap<QString, QVariant> getVocabularySynonyms( const QModelIndex &index ) const;
		QMap<QString, QVariant> getArchiveSynonyms( const QModelIndex &index ) const;
		QString getOnlyWordsSynonyms( const QModelIndex &index ) const;

	signals:
		void modelUpdated();
};

#endif	// MODEL_VOCABULARYWORDS_H
