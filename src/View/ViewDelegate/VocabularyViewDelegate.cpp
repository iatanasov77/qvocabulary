#include "VocabularyViewDelegate.h"

#include <QApplication>
#include <QLineEdit>
#include <QIcon>
#include <QMouseEvent>
#include <QDebug>

#include "View/VocabularyTableView.h"

VocabularyViewDelegate::VocabularyViewDelegate( QObject *parent ) : QStyledItemDelegate( parent )
{
	view	= parent;
}

QRect VocabularyViewDelegate::textRect( QRect cellRect ) const
{
	int btnSize	= cellRect.height();	// Button rect should be a square

	int x 		= cellRect.left();
	int y 		= cellRect.top();
	int w 		= cellRect.width() - btnSize;
	int h 		= cellRect.height();

	return QRect( x, y, w, h );
}

QRect VocabularyViewDelegate::buttonRect( QRect cellRect ) const
{
	int btnSize	= cellRect.height();	// Button rect should be a square

	int x = cellRect.left() + cellRect.width() - btnSize;
	int y = cellRect.top();
	int w = btnSize;
	int h = btnSize;

	return QRect( x, y, w, h );
}

void VocabularyViewDelegate::setCursor( QEvent *event, const QStyleOptionViewItem option, const QModelIndex index ) const
{
	QMouseEvent *e			= static_cast<QMouseEvent *>( event );
	QPointF mousePosition	= e->pos();

	if ( buttonRect( option.rect ).contains( mousePosition.toPoint() ) ) {
		//qDebug() << "SET CURSOR !!!";
		QApplication::setOverrideCursor( QCursor( Qt::PointingHandCursor ) );
		//option.widget->setCursor( QCursor( Qt::PointingHandCursor ) );
	} else {
		//qDebug() << "RESTORE CURSOR !!!";
		QApplication::restoreOverrideCursor();
	}
}

/*
 * Manual: https://stackoverflow.com/questions/9131727/how-can-i-change-the-mouse-pointer-when-mouse-over-text-with-qstyleditemdelegate
 */
void VocabularyViewDelegate::setCursor( const QStyleOptionViewItem option, const QModelIndex index ) const
{
	VocabularyTableView *myView	= qobject_cast<VocabularyTableView*>( view );
	QPoint mousePosition		= QCursor::pos();
	QPoint widgetPos 			= option.widget->mapFromGlobal( mousePosition );

	/*
	 * Notice that you may need the viewport() in order to be precise. In this case in order to map the global coordinates you will need:
	 */
	//QPoint viewportPos = myWidget->viewport()->mapFromGlobal( globalCursorPos );
	if ( buttonRect( option.rect ).contains( mousePosition ) ) {
	     QModelIndex currentIndex	= myView->indexAt( widgetPos );

	     if ( currentIndex == index )
	         QApplication::setOverrideCursor( QCursor( Qt::PointingHandCursor ) );
	     else
	         QApplication::restoreOverrideCursor();
	} else {
        QApplication::restoreOverrideCursor();
	}
}

bool VocabularyViewDelegate::isEmptyLine( const QModelIndex index ) const
{
	if ( index.siblingAtColumn( 0 ).data().toInt() )
		return false;
	else
		return true;
}
