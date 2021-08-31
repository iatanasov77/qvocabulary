#ifndef MODEL_WORDSLIST_H
#define MODEL_WORDSLIST_H

#include <QAbstractListModel>

class WordsListModel : public QAbstractListModel
{
	Q_OBJECT

	public :
  		WordsListModel( QAbstractItemModel *sourceModel, QList<int> selectedItems, QObject * parent = 0 );
  		int rowCount( const QModelIndex& parent = QModelIndex() ) const;
  		QVariant data( const QModelIndex &index, int role ) const;

  		virtual bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );
	private:
  		QAbstractItemModel *source;
  		QList<int> selected;

  		void populate();
};

#endif	// MODEL_WORDSLIST_H
