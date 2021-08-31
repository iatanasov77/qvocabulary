#ifndef VS_WIDGET_DELEGATE_MULTISELECTCOMBOBOXDELEGATE_H
#define VS_WIDGET_DELEGATE_MULTISELECTCOMBOBOXDELEGATE_H

#include <QItemDelegate>
#include <QStyledItemDelegate>

class VsMultiSelectComboBoxDelegate : public QItemDelegate
{
	Q_OBJECT

	public:
		VsMultiSelectComboBoxDelegate( QObject *parent );

		void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
		QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
		void setEditorData( QWidget *editor, const QModelIndex &index ) const;
		void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;
		void updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
 };

#endif // VS_WIDGET_DELEGATE_MULTISELECTCOMBOBOXDELEGATE_H
