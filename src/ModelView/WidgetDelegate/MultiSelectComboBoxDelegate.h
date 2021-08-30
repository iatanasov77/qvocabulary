#ifndef MULTISELECTCOMBOBOX_DELEGATE_H
#define MULTISELECTCOMBOBOX_DELEGATE_H

#include <QItemDelegate>
#include <QStyledItemDelegate>

class MultiSelectComboBoxDelegate : public QItemDelegate
{
	Q_OBJECT

	public:
		MultiSelectComboBoxDelegate( QObject *parent );

		void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
		QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
		void setEditorData( QWidget *editor, const QModelIndex &index ) const;
		void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;
		void updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
 };

#endif // MULTISELECTCOMBOBOX_DELEGATE_H
