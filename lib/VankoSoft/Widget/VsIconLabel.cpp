#include "VsIconLabel.h"

#include <QLabel>
#include <QIcon>
#include <QSize>
#include <QHBoxLayout>

VsIconLabel::VsIconLabel( QString iconPath, QString text, bool finalStretch, QWidget* parent ) : QWidget( parent )
{
	QSize iconSize			= QSize( 16, 16 );
	int horizontalSpacing	= 2;

	QHBoxLayout *layout = new QHBoxLayout( this );
	layout->setContentsMargins( 0, 0, 0, 0 );
	//setLayout( layout );

	QLabel *icon				= new QLabel();
	//QIcon dictionary( ":/Resources/icons/dictionary.svg" );
	QIcon dictionary( iconPath );
	icon->setPixmap( dictionary.pixmap( iconSize ) );

	layout->addWidget( icon );
	layout->addSpacing( horizontalSpacing );
	layout->addWidget( new QLabel( text ) );

	if ( finalStretch )
		layout->addStretch();
}

VsIconLabel::~VsIconLabel() {}
