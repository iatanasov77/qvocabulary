#ifndef MODEL_VOCABULARYWORDS_H
#define MODEL_VOCABULARYWORDS_H

#include <QObject>

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
};

#endif	// MODEL_VOCABULARYWORDS_H
