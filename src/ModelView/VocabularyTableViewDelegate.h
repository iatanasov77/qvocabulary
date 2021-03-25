#ifndef VIEW_VOCABULARYTABLEVIEWDELEGATE_H
#define VIEW_VOCABULARYTABLEVIEWDELEGATE_H

#include <QStyledItemDelegate>

class VocabularyTableViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT

	public:
		VocabularyTableViewDelegate( QObject *parent = 0 );
		virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
		bool editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index );
		QWidget* createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

	private:
		QRect textRect( QRect cellRect ) const;
		QRect buttonRect( QRect cellRect ) const;
		QStyleOptionButton createButton( QRect rect  ) const;

	signals:
		void buttonClicked( const QModelIndex &index );
};

#endif	// VIEW_VOCABULARYTABLEVIEWDELEGATE_H
