#include "VocabularyTableViewDelegate.h"

#include <QApplication>
#include <QLineEdit>
#include <QIcon>
#include <QMouseEvent>
#include <QDebug>

VocabularyTableViewDelegate::VocabularyTableViewDelegate( QObject *parent ) : QStyledItemDelegate( parent )
{

}

void VocabularyTableViewDelegate::paint(
	QPainter *painter,
	const QStyleOptionViewItem &option,
	const QModelIndex &index
) const {
	QStyleOptionViewItem op( option );
	initStyleOption( &op, index );
	op.font.setBold( true );

	QStyleOptionButton button	= createButton( buttonRect( option.rect ) );
	QStyle *style				= option.widget ? option.widget->style() : QApplication::style();

	style->drawControl( QStyle::CE_PushButton, &button, painter, option.widget );

	op.rect	= textRect( option.rect );
	QStyledItemDelegate::paint( painter, op, index );
}

bool VocabularyTableViewDelegate::editorEvent(
	QEvent *event,
	QAbstractItemModel *model,
	const QStyleOptionViewItem &option,
	const QModelIndex &index
) {
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

	if ( event->type() == QEvent::MouseButtonDblClick ) {
//		QRect editorRect	= textRect( option.rect );
//		option.rect			= editorRect;
	}

	return QStyledItemDelegate::editorEvent( event, model, option, index );
}

QWidget* VocabularyTableViewDelegate::createEditor(
		QWidget *parent,
		const QStyleOptionViewItem &option,
		const QModelIndex &index
) const {
	Q_UNUSED( index );

	qDebug() << "CREATE EDITOR";

	QRect editorRect	= textRect( option.rect );
	QLineEdit* editor	= new QLineEdit( parent );
	//editor->setGeometry( editorRect );
	editor->resize( editorRect.width(), editorRect.height() );
	//connect( editor, SIGNAL( editingFinished() ), this, SLOT( commitAndCloseEditor() ) );

	return editor;
 }

QStyleOptionButton VocabularyTableViewDelegate::createButton( QRect buttonRect  ) const
{
	QStyleOptionButton button;

	button.rect		= buttonRect;
	button.icon 	= QIcon( ":/Resources/icons/speaker.png" );
	button.iconSize	= QSize( 16, 16 );
	button.state 	= QStyle::State_Enabled;

	return button;
}

QRect VocabularyTableViewDelegate::textRect( QRect cellRect ) const
{
	int btnSize	= cellRect.height();

	int x 		= cellRect.left();
	int y 		= cellRect.top();
	int w 		= cellRect.width() - btnSize;
	int h 		= cellRect.height();

	return QRect( x, y, w, h );
}

QRect VocabularyTableViewDelegate::buttonRect( QRect cellRect ) const
{
	int btnSize	= cellRect.height();

	int x = cellRect.left() + cellRect.width() - btnSize;
	int y = cellRect.top();
	int w = btnSize;
	int h = btnSize;

	return QRect( x, y, w, h );
}
