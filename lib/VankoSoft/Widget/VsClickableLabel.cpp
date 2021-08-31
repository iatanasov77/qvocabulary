#include "VsClickableLabel.h"

#include <QVariant>

VsClickableLabel::VsClickableLabel( QWidget* parent, Qt::WindowFlags f ) : QLabel( parent )
{

}

VsClickableLabel::~VsClickableLabel() {}

void VsClickableLabel::mousePressEvent( QMouseEvent* event ) {
    emit clicked( property( "labelData" ) );
}
