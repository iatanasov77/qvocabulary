#ifndef VIEW_VOCABULARYTABLEVIEWDELEGATE_H
#define VIEW_VOCABULARYTABLEVIEWDELEGATE_H

#include <QStyledItemDelegate>

class VocabularyTableViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT

	public:
		VocabularyTableViewDelegate( QObject *parent = 0 );
		virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
};

#endif	// VIEW_VOCABULARYTABLEVIEWDELEGATE_H
