#include "MultiSelectComboBox.h"

#include <QComboBox>
#include <QCoreApplication>
#include <QAbstractItemView>
#include <QCheckBox>
#include <QStylePainter>
#include <QListView>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QDebug>

#include "ModelView/WidgetDelegate/MultiSelectComboBoxDelegate.h"

/**
 * QComboBox with Checkboxes: https://gist.github.com/mistic100/c3b7f3eabc65309687153fe3e0a9a720
 */
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

    connect( ( QListView* ) view(), SIGNAL( pressed( QModelIndex ) ), this, SLOT( onItemPressed( QModelIndex ) ) );
}

MultiSelectComboBox::~MultiSelectComboBox()
{

}

bool MultiSelectComboBox::eventFilter( QObject *object, QEvent *event )
{
    // don't close items view after we release the mouse button
    // by simple eating MouseButtonRelease in viewport of items view
    if( event->type() == QEvent::MouseButtonRelease && object == view()->viewport() )
    {
        return true;
    }

    return QComboBox::eventFilter( object, event );
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

void MultiSelectComboBox::onItemPressed ( const QModelIndex &index )
{
	model()->setData( index, 0, Qt::CheckStateRole );
}

void MultiSelectComboBox::setFilterText( const QString &text )
{
	//qDebug() << "Filter Text Changed: " << text;
	_lineEdit->setText( text );
	emit editTextChanged( text );
}

void MultiSelectComboBox::setEditable( bool editable )
{
	QComboBox::setEditable( editable );
	_lineEdit	= new QLineEdit( this );
	setLineEdit( _lineEdit );

	installEventFilter( this );
	_lineEdit->installEventFilter( this );

	connect( _lineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( setFilterText( const QString & ) ) );
	connect( this, SIGNAL( editTextChanged( QString ) ), ( QSortFilterProxyModel* ) model(), SLOT( setFilterWildcard( QString ) ) );

	_lineEdit->setPlaceholderText( m_DisplayText );
	_lineEdit->setText( "" );
	emit editTextChanged( _lineEdit->text() );
}
