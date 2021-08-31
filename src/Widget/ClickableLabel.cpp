#include "ClickableLabel.h"

#include <QVariant>

ClickableLabel::ClickableLabel( QWidget* parent, Qt::WindowFlags f ) : QLabel( parent )
{

}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent( QMouseEvent* event ) {
    emit clicked( property( "labelData" ) );
}
