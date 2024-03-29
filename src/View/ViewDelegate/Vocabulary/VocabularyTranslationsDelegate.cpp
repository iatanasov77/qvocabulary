#include "VocabularyTranslationsDelegate.h"

#include <QApplication>
#include <QLineEdit>
#include <QIcon>
#include <QMouseEvent>
#include <QDebug>

#include "View/VocabularyTableView.h"

VocabularyTranslationsDelegate::VocabularyTranslationsDelegate( QObject *parent ) : VocabularyViewDelegate( parent )
{

}

void VocabularyTranslationsDelegate::paint(
	QPainter *painter,
	const QStyleOptionViewItem &option,
	const QModelIndex &index
) const {
	if ( ! index.isValid() || isEmptyLine( index ) )
		QStyledItemDelegate::paint( painter, option, QModelIndex() );	// draw the selection background only

	//setCursor( option, index );

	QStyleOptionViewItem op( option );
	initStyleOption( &op, index );

	QStyleOptionButton button	= createButton( buttonRect( option.rect ) );
	QStyle *style				= option.widget ? option.widget->style() : QApplication::style();

	if ( ! isEmptyLine( index ) )
		style->drawControl( QStyle::CE_PushButton, &button, painter, option.widget );

	op.rect	= textRect( option.rect );
	QStyledItemDelegate::paint( painter, op, index );
}

bool VocabularyTranslationsDelegate::editorEvent(
	QEvent *event,
	QAbstractItemModel *model,
	const QStyleOptionViewItem &option,
	const QModelIndex &index
) {
	setCursor( event, option, index );

	if ( event->type() == QEvent::MouseButtonRelease ) {
		QMouseEvent* e	= ( QMouseEvent* )event;
		int clickX		= e->x();
		int clickY		= e->y();

		QRect btnRect	= buttonRect( option.rect );
		if(
			( clickX > btnRect.x() && clickX < btnRect.x() + btnRect.width() ) &&
			( clickY > btnRect.y() && clickY < btnRect.y() + btnRect.height() )
		) {
			emit buttonClicked( index );
		}
	}

	return QStyledItemDelegate::editorEvent( event, model, option, index );
}

QWidget* VocabularyTranslationsDelegate::createEditor(
		QWidget *parent,
		const QStyleOptionViewItem &option,
		const QModelIndex &index
) const {
	Q_UNUSED( index );

	QRect editorRect	= textRect( option.rect );
	QLineEdit* editor	= new QLineEdit( parent );

	editor->resize( editorRect.width(), editorRect.height() );

	return editor;
 }

QStyleOptionButton VocabularyTranslationsDelegate::createButton( QRect buttonRect  ) const
{
	QStyleOptionButton button;

	button.rect		= buttonRect;
	button.icon 	= QIcon( ":/Resources/icons/dictionary-30.svg" );
	button.iconSize	= QSize( 16, 16 );
	button.state 	= QStyle::State_Enabled;

	return button;
}
