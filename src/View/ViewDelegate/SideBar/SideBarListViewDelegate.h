#ifndef VIEW_SIDEBARLISTVIEWDELEGATE_H
#define VIEW_SIDEBARLISTVIEWDELEGATE_H

#include <QItemDelegate>
#include <QSize>
#include <QAbstractItemModel>

/*
 * https://kunalmaemo.blogspot.com/2010/12/creating-custom-qitemdelegate-with.html
 */
class SideBarListViewDelegate : public QItemDelegate
{
    Q_OBJECT

	public:
		SideBarListViewDelegate( QAbstractItemModel *model, int currRow = 0, bool inArchive = false, QObject *parent = 0 );
		virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
		virtual QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const;
		bool editorEvent( QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index );

		void setEvent( int event );

	protected:
		void setCursor( QEvent *event, const QStyleOptionViewItem option, const QModelIndex index ) const;

	signals:
		void buttonClicked( const QModelIndex &index );
		void quizButtonClicked( const QModelIndex &index );

	private:
		QStyleOptionButton createButton( int indexRow, QRect rect, QString text ) const;
		QStyleOptionButton createQuizButton( int indexRow, QRect rect ) const;
		long groupWordsCount( int groupId ) const;
		QRect groupButtonRect( QRect cellRect ) const;
		QRect quizButtonRect( QRect cellRect ) const;

		bool _inArchive;
		int _currRow;
		int _event;	// Store: QEvent::Type
		bool _showAllButton;
};

#endif	// VIEW_SIDEBARLISTVIEWDELEGATE_H
