#ifndef VIEW_VOCABULARYTRANSCRIPTIONSDELEGATE_H
#define VIEW_VOCABULARYTRANSCRIPTIONSDELEGATE_H

#include "View/ViewDelegate/VocabularyViewDelegate.h"

class VocabularyTranscriptionsDelegate : public VocabularyViewDelegate
{
    Q_OBJECT

	public:
		VocabularyTranscriptionsDelegate( QObject *parent = nullptr );
		virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
		bool editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index );
		QWidget* createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

	protected:
		QStyleOptionButton createButton( QRect rect  ) const;
};

#endif	// VIEW_VOCABULARYTRANSCRIPTIONSDELEGATE_H
