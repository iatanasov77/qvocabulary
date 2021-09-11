#include "ArchiveCompareTableView.h"

#include <QSizePolicy>
#include <QDialog>
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QTableView>
#include <QVBoxLayout>
#include <QHeaderView>

/**
 * Tutorial: https://stackoverflow.com/questions/46431491/qtableview-to-show-the-content-of-table-item-when-mouse-hover/53199540
 *
 */
ArchiveCompareTableView::ArchiveCompareTableView( QWidget *parent ) : QTableView( parent )
{
	viewport()->installEventFilter( this );
	setMouseTracking( true );
	popup				= new QDialog( this, Qt::Popup | Qt::ToolTip );

	QVBoxLayout *layout = new QVBoxLayout;
	popupLabel 			= new QLabel( popup );
	popupLabel->setWordWrap( true );
	layout->addWidget( popupLabel );
	popupLabel->setTextFormat( Qt::RichText );
	//popupLabel->setOpenExternalLinks( true );
	popup->setLayout( layout );
	popup->installEventFilter( this );
}

bool ArchiveCompareTableView::eventFilter( QObject *watched, QEvent *event )
{
	QString popupText;

	if ( viewport() == watched ) {
		if( event->type() == QEvent::MouseMove ) {
			QMouseEvent *mouseEvent	= static_cast<QMouseEvent*>( event );
			QModelIndex index 		= indexAt( mouseEvent->pos() );
			if( index.isValid() && ( index.column() == 0 || index.column() == 1 ) ) {
				popupText	= QString( "Click to Open the Word in Vocabulary." );
				showPopup( index, popupText );
			} else if ( index.isValid() && ( index.column() == 2 || index.column() == 3 ) ) {
				popupText	= QString( "Click to Open the Word in Archive." );
				showPopup( index, popupText );
			} else{
				popup->hide();
			}
		}
		else if ( event->type() == QEvent::Leave ) {
			popup->hide();
		}
	}
	else if ( popup == watched ) {
		if( event->type() == QEvent::Leave ) {
			popup->hide();
		}
	}
	return QTableView::eventFilter( watched, event );
}

void ArchiveCompareTableView::showPopup( const QModelIndex &index, QString popupText ) const
{
	// May be should get the column width
	int popupWidth	= 300;

	QRect r	= visualRect( index );
	popup->move( viewport()->mapToGlobal( r.bottomLeft() ) );
	popup->setFixedSize( popupWidth, popup->heightForWidth( popupWidth ) );

	popupLabel->setText( popupText );
	popup->adjustSize();
	popup->show();
}
