#include "MultiSelectComboBox.h"

#include <QAbstractItemView>
#include <QCheckBox>
#include <QStylePainter>

#include "ModelView/WidgetDelegate/MultiSelectComboBoxDelegate.h"

MultiSelectComboBox::MultiSelectComboBox( QWidget *widget ) : QComboBox( widget )
{
    // set delegate items view
    view()->setItemDelegate( new MultiSelectComboBoxDelegate( this ) );
    //view()->setStyleSheet("  padding: 15px; ");
    // Enable editing on items view
    view()->setEditTriggers( QAbstractItemView::CurrentChanged );

    // set "MultiSelectComboBox::eventFilter" as event filter for items view
    view()->viewport()->installEventFilter( this );

    // it just cool to have it as defualt ;)
    view()->setAlternatingRowColors( true );
}

MultiSelectComboBox::~MultiSelectComboBox()
{

}

bool MultiSelectComboBox::eventFilter( QObject *object, QEvent *event )
{
    // don't close items view after we release the mouse button
    // by simple eating MouseButtonRelease in viewport of items view
    if( event->type() == QEvent::MouseButtonRelease && object==view()->viewport() )
    {
        return true;
    }
    return QComboBox::eventFilter( object,event );
}

void MultiSelectComboBox::paintEvent( QPaintEvent * )
{
    QStylePainter painter( this );
    painter.setPen( palette().color( QPalette::Text ) );

    // draw the combobox frame, focusrect and selected etc.
    QStyleOptionComboBox opt;
    initStyleOption( &opt );

    // if no display text been set , use "..." as default
    if( m_DisplayText.isNull() )
        opt.currentText = "......";
    else
        opt.currentText = m_DisplayText;

    painter.drawComplexControl( QStyle::CC_ComboBox, opt );

    // draw the icon and text
    painter.drawControl( QStyle::CE_ComboBoxLabel, opt );

}

void MultiSelectComboBox::setDisplayText( QString text )
{
    m_DisplayText = text;
}

QString MultiSelectComboBox::getDisplayText() const
{
    return m_DisplayText;
}
