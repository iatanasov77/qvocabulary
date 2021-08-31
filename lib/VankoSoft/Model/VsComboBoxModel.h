#ifndef VS_MODEL_COMBOBOXMODEL_H
#define VS_MODEL_COMBOBOXMODEL_H

#include <QAbstractListModel>

class VsComboBoxModel : public QAbstractListModel
{
	Q_OBJECT

	public :
  		VsComboBoxModel( QAbstractItemModel *sourceModel, QList<int> selectedItems, QObject * parent = 0 );
  		int rowCount( const QModelIndex& parent = QModelIndex() ) const;
  		QVariant data( const QModelIndex &index, int role ) const;

  		virtual bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );
	private:
  		QAbstractItemModel *source;
  		QList<int> selected;

  		void populate();
};

#endif	// VS_MODEL_COMBOBOXMODEL_H
