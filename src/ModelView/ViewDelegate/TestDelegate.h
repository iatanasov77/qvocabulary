#ifndef VIEW_TESTDELEGATE_H
#define VIEW_TESTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class TestDelegate : public QStyledItemDelegate
{
    Q_OBJECT

	public:
		TestDelegate( QObject *parent = 0 );
		virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

	private:
		static int countWords;

		QRect widgetRect( QRect cellRect, int widgetNumber = 1 ) const;
		void createWidget( QPainter *painter, QStyleOptionViewItem op, QModelIndex index, QString text ) const;
};

#endif	// VIEW_TESTDELEGATE_H
