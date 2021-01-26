#include "VocabularyWordsTableView.h"

#include <QDebug>

VocabularyWordsTableView::VocabularyWordsTableView( QWidget* parent ) : QTableView( parent )
{
	setAcceptDrops( true );
	setDropIndicatorShown( true );
}

VocabularyWordsTableView::~VocabularyWordsTableView()
{
}

void VocabularyWordsTableView::dragEnterEvent( QDragEnterEvent* event )
{
    event->accept();
}

void VocabularyWordsTableView::dragMoveEvent( QDragMoveEvent* event )
{
	qDebug() << "DRAG ACCEPTED 1";
	event->accept();
}

void VocabularyWordsTableView::dragLeaveEvent(QDragLeaveEvent *event)
{

    event->accept();
}

void VocabularyWordsTableView::dropEvent ( QDropEvent* event )
{
	qDebug() << "DROP ACCEPTED 1";
	event->accept();
	if ( event ) {
		qDebug() << "DROP ACCEPTED 2";
		QTableView* table	= qobject_cast<QTableView*>(event->source());
		if (table) {
//			QString item = ""// Decode MIME data here.
//			Qt::MatchFlag someFlag = Qt::MatchExactly; // Check documentation for match type.
//			QList<QTableWidgetItem *> items = table->findItems(item, someFlag)
//			// If you don't have repeats, items[0] is what you want.
//			int initialRow = table->row(items[0]);
		}
	}
}
