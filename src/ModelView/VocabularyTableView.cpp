#include "VocabularyTableView.h"

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
VocabularyTableView::VocabularyTableView( QWidget *parent ) : QTableView( parent )
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

bool VocabularyTableView::eventFilter( QObject *watched, QEvent *event )
{
	if ( viewport() == watched ) {
		if( event->type() == QEvent::MouseMove ) {
			QMouseEvent *mouseEvent	= static_cast<QMouseEvent*>( event );
			QModelIndex index 		= indexAt( mouseEvent->pos() );
			if( index.isValid() ) {
				showPopup( index );
			}
			else{
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

void VocabularyTableView::showPopup( const QModelIndex &index ) const
{
	if ( index.column() == 1 ) {
		QString description	= index.siblingAtColumn( 5 ).data( Qt::DisplayRole ).toString();
		if ( description.size() > 0 ) {
			QRect r	= visualRect( index );
			popup->move( viewport()->mapToGlobal( r.bottomLeft() ) );
			popup->setFixedSize( 100, popup->heightForWidth( 100 ) );

			// Sibling(5) = description
			popupLabel->setText( description );
			popup->adjustSize();
			popup->show();
		}
	}
	else {
		popup->hide();
	}
}
