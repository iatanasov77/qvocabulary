#include "VsClickableLabel.h"

#include <QVariant>

VsClickableLabel::VsClickableLabel( QWidget *parent, Qt::WindowFlags f ) : QLabel( parent )
{
	Q_UNUSED( f );
}

VsClickableLabel::~VsClickableLabel() {}

void VsClickableLabel::mousePressEvent( QMouseEvent *event )
{
	Q_UNUSED( event );

    emit clicked( property( "labelData" ) );
}
