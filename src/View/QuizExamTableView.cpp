#include "QuizExamTableView.h"

#include <QSizePolicy>
#include <QDialog>
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHeaderView>

// From: https://github.com/robertknight/qt-mustache
#include "../lib/QtMustache/mustache.h"
#include "Application/VsApplication.h"

/**
 * Tutorial: https://stackoverflow.com/questions/46431491/qtableview-to-show-the-content-of-table-item-when-mouse-hover/53199540
 *
 */
QuizExamTableView::QuizExamTableView( QWidget *parent ) : QTableView( parent )
{
	viewport()->installEventFilter( this );
	setMouseTracking( true );

	initHeader();
	initPopup();
}

bool QuizExamTableView::eventFilter( QObject *watched, QEvent *event )
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

void QuizExamTableView::initHeader()
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

void QuizExamTableView::initPopup()
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

	// May be should get the column width
	int popupWidth	= 300;
	popup->setFixedSize( popupWidth, popup->heightForWidth( popupWidth ) );
}

void QuizExamTableView::showPopup( const QModelIndex &index ) const
{
	if ( index.column() == 2 ) {
		QRect r	= visualRect( index );
		popup->move( viewport()->mapToGlobal( r.bottomLeft() ) );

		popupLabel->setText( popupText( index ) );
		popup->adjustSize();
		popup->show();
	}
	else {
		popup->hide();
	}
}

QString QuizExamTableView::popupText( const QModelIndex &index  ) const
{
	QVariantHash variables;
	variables["transcription"]	= index.siblingAtColumn( 6 ).data( Qt::DisplayRole ).toString();
	variables["description"]	= index.siblingAtColumn( 7 ).data( Qt::DisplayRole ).toString();
	variables["synonyms"]		= index.siblingAtColumn( 8 ).data( Qt::DisplayRole ).toString();

	Mustache::Renderer renderer;
	Mustache::QtVariantContext context( variables );
	return renderer.render( VsApplication::instance()->quizHelpDataBody(), &context );
}
