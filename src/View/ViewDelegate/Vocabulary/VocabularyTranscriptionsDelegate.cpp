#include "VocabularyTranscriptionsDelegate.h"

#include <QApplication>
#include <QLineEdit>
#include <QIcon>
#include <QMouseEvent>
#include <QDebug>

#include "View/VocabularyTableView.h"

VocabularyTranscriptionsDelegate::VocabularyTranscriptionsDelegate( QObject *parent ) : QStyledItemDelegate( parent )
{
	view	= parent;
}

void VocabularyTranscriptionsDelegate::paint(
	QPainter *painter,
	const QStyleOptionViewItem &option,
	const QModelIndex &index
) const {
	if ( ! index.isValid() || isEmptyLine( index ) )
		QStyledItemDelegate::paint( painter, option, QModelIndex() );	// draw the selection background only

	//setCursor( option, index );
	QStyleOptionViewItem op( option );
	initStyleOption( &op, index );
	op.font.setBold( true );

	QStyleOptionButton button	= createButton( buttonRect( option.rect ) );
	QStyle *style				= option.widget ? option.widget->style() : QApplication::style();

	//option.widget->setCursor( QCursor( Qt::PointingHandCursor ) );
	if ( ! isEmptyLine( index ) )
		style->drawControl( QStyle::CE_PushButton, &button, painter, option.widget );

	op.rect	= textRect( option.rect );
	QStyledItemDelegate::paint( painter, op, index );
}

bool VocabularyTranscriptionsDelegate::editorEvent(
	QEvent *event,
	QAbstractItemModel *model,
	const QStyleOptionViewItem &option,
	const QModelIndex &index
) {
	bool hover = false;
	if ( option.state & QStyle::State_MouseOver )
		hover = true;
	qDebug() << "BUTTON IS HOVERED: " << hover;

	if ( event->type() == QEvent::MouseButtonRelease ) {
		QMouseEvent* e	= ( QMouseEvent* )event;
		int clickX		= e->x();
		int clickY		= e->y();

		// parent->setCursor( QCursor( Qt::PointingHandCursor ) );
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

QWidget* VocabularyTranscriptionsDelegate::createEditor(
		QWidget *parent,
		const QStyleOptionViewItem &option,
		const QModelIndex &index
) const {
	Q_UNUSED( index );

	QRect editorRect	= textRect( option.rect );
	QLineEdit* editor	= new QLineEdit( parent );
	//editor->setGeometry( editorRect );
	editor->resize( editorRect.width(), editorRect.height() );
	//connect( editor, SIGNAL( editingFinished() ), this, SLOT( commitAndCloseEditor() ) );

	return editor;
 }

QStyleOptionButton VocabularyTranscriptionsDelegate::createButton( QRect buttonRect  ) const
{
	QStyleOptionButton button;

	button.rect		= buttonRect;
	button.icon 	= QIcon( ":/Resources/icons/speaker.png" );
	button.iconSize	= QSize( 16, 16 );
	button.state 	= QStyle::State_Enabled;

	return button;
}

QRect VocabularyTranscriptionsDelegate::textRect( QRect cellRect ) const
{
	int btnSize	= cellRect.height();	// Button rect should be a square

	int x 		= cellRect.left();
	int y 		= cellRect.top();
	int w 		= cellRect.width() - btnSize;
	int h 		= cellRect.height();

	return QRect( x, y, w, h );
}

QRect VocabularyTranscriptionsDelegate::buttonRect( QRect cellRect ) const
{
	int btnSize	= cellRect.height();	// Button rect should be a square

	int x = cellRect.left() + cellRect.width() - btnSize;
	int y = cellRect.top();
	int w = btnSize;
	int h = btnSize;

	return QRect( x, y, w, h );
}

/*
 * Manual: https://stackoverflow.com/questions/9131727/how-can-i-change-the-mouse-pointer-when-mouse-over-text-with-qstyleditemdelegate
 */
void VocabularyTranscriptionsDelegate::setCursor( const QStyleOptionViewItem option, const QModelIndex index ) const
{
	VocabularyTableView *myView	= qobject_cast<VocabularyTableView*>( view );

	/*
	 * Notice that you may need the viewport() in order to be precise. In this case in order to map the global coordinates you will need:
	 */
	//QPoint viewportPos = myWidget->viewport()->mapFromGlobal( globalCursorPos );

	if ( option.state & QStyle::State_MouseOver ) {
	     QPoint globalCursorPos		= QCursor::pos();
	     QPoint widgetPos 			= option.widget->mapFromGlobal( globalCursorPos );
	     QModelIndex currentIndex	= myView->indexAt( widgetPos );

	     if ( currentIndex == index )
	         QApplication::setOverrideCursor( QCursor( Qt::PointingHandCursor ) );
	     else
	         QApplication::restoreOverrideCursor();
	}
}

bool VocabularyTranscriptionsDelegate::isEmptyLine( const QModelIndex index ) const
{
	if ( index.siblingAtColumn( 0 ).data().toInt() )
		return false;
	else
		return true;
}
