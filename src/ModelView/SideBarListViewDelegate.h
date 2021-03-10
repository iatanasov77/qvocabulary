#ifndef VIEW_SIDEBARLISTVIEWDELEGATE_H
#define VIEW_SIDEBARLISTVIEWDELEGATE_H

#include <QItemDelegate>
#include <QSize>

/*
 * https://kunalmaemo.blogspot.com/2010/12/creating-custom-qitemdelegate-with.html
 */
class SideBarListViewDelegate : public QItemDelegate
{
    Q_OBJECT

	public:
		SideBarListViewDelegate( int currRow = 0, QObject *parent = 0 );
		virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
		virtual QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const;
		bool editorEvent( QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index );

	signals:
		void buttonClicked( const QModelIndex &index );

	private:
		int _currRow;
		int _event;	// 1 for right click
};

#endif	// VIEW_SIDEBARLISTVIEWDELEGATE_H
