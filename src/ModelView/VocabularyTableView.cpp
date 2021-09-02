#include "VocabularyTableView.h"

#include <QSizePolicy>
#include <QDialog>
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QTableView>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

/**
 * Tutorial: https://stackoverflow.com/questions/46431491/qtableview-to-show-the-content-of-table-item-when-mouse-hover/53199540
 *
 */
VocabularyTableView::VocabularyTableView( QWidget *parent ) : QTableView( parent )
{
	viewport()->installEventFilter( this );
	setMouseTracking( true );

	initHeader();
	initPopup();
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

void VocabularyTableView::initHeader()
{
	if ( QSysInfo::windowsVersion() == QSysInfo::WV_WINDOWS10 ) {
	    setStyleSheet(
	        "QHeaderView::section{"
	            "border-top:0px solid #D8D8D8;"
	            "border-left:0px solid #D8D8D8;"
	            "border-right:1px solid #D8D8D8;"
	            "border-bottom: 1px solid #D8D8D8;"
	            "background-color:white;"
	            "padding:4px;"
	        "}"
	        "QTableCornerButton::section{"
	            "border-top:0px solid #D8D8D8;"
	            "border-left:0px solid #D8D8D8;"
	            "border-right:1px solid #D8D8D8;"
	            "border-bottom: 1px solid #D8D8D8;"
	            "background-color:white;"
	        "}"
	    );
	}
}

void VocabularyTableView::initPopup()
{
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

void VocabularyTableView::showPopup( const QModelIndex &index ) const
{
	// May be should get the column width
	int popupWidth	= 300;

	if ( index.column() == 1 ) {
		QString description	= index.siblingAtColumn( 5 ).data( Qt::DisplayRole ).toString();
		if ( description.size() > 0 ) {
			QRect r	= visualRect( index );
			popup->move( viewport()->mapToGlobal( r.bottomLeft() ) );
			popup->setFixedSize( popupWidth, popup->heightForWidth( popupWidth ) );

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

void VocabularyTableView::displayWord( QVariant wordId )
{
	QMessageBox::warning( this, tr( "Debug Message" ),
		tr( "Word ID: %1." )
		.arg( QString::number( wordId.toInt() ) )
	);
}
