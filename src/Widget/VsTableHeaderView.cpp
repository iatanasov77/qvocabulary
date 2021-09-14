#include "VsTableHeaderView.h"

QMap<int, QRect> VsTableHeaderView::_checkboxRects;

VsTableHeaderView::VsTableHeaderView( Qt::Orientation orientation, QWidget *parent, QList<int> checkboxColumns ) :
	QHeaderView( orientation, parent)
{
	initStyle();

	_checkboxColumns	= checkboxColumns;
	_checkboxRects		= QMap<int, QRect>();
	foreach( int section, _checkboxColumns ) {
		isOn[section]	= false;
	}

	connect( this, SIGNAL( checkboxClicked( int, bool ) ), this, SLOT( checkAll( int, bool ) ) );
}

void VsTableHeaderView::initStyle()
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

void VsTableHeaderView::paintSection( QPainter *painter, const QRect &rect, int logicalIndex ) const
{
	painter->save();
	QHeaderView::paintSection( painter, rect, logicalIndex );
	painter->restore();

	if ( _checkboxColumns.contains( logicalIndex ) ) {
		QStyleOptionButton option;
		option.rect	= QRect( 2 + rect.x(), ( rect.height() / 2 ) - 7, 15, 15 );
		VsTableHeaderView::_checkboxRects[logicalIndex]	= option.rect;
		if ( isOn[logicalIndex] )
			option.state	= QStyle::State_On;
		else
			option.state	= QStyle::State_Off;

		this->style()->drawPrimitive( QStyle::PE_IndicatorCheckBox, &option, painter );
	}
}

void VsTableHeaderView::mousePressEvent( QMouseEvent *e )
{
	QRect rect;
	foreach ( int logicalIndex, VsTableHeaderView::_checkboxRects.keys() ) {
		rect	= VsTableHeaderView::_checkboxRects[logicalIndex];
		if(
			( e->x() > rect.x() && e->x() < rect.x() + rect.width() ) &&
			( e->y() > rect.y() && e->y() < rect.y() + rect.height() )
		) {
			if ( isOn[logicalIndex] )
				isOn[logicalIndex]	= false;
			else
				isOn[logicalIndex]	= true;

			emit checkboxClicked( logicalIndex, isOn[logicalIndex] );
			updateSection( logicalIndex );
		}
	}

	this->update();
	QHeaderView::mousePressEvent( e );
}

void VsTableHeaderView::checkAll( int logicalIndex, bool isOn )
{
	this->isOn[logicalIndex]	= isOn;
	//updateSection( logicalIndex );
}
