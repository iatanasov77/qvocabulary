#ifndef VIEW_VOCABULARYTRANSLATIONS_DELEGATE_H
#define VIEW_VOCABULARYTRANSLATIONS_DELEGATE_H

#include "View/ViewDelegate/VocabularyViewDelegate.h"

class VocabularyTranslationsDelegate : public VocabularyViewDelegate
{
    Q_OBJECT

	public:
		VocabularyTranslationsDelegate( QObject *parent = 0 );
		virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
		bool editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index );
		QWidget* createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

	protected:
		QStyleOptionButton createButton( QRect rect  ) const;
};

#endif	// VIEW_VOCABULARYTRANSLATIONS_DELEGATE_H
