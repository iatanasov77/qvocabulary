#ifndef VS_WIDGET_CLICKABLELABEL_H
#define VS_WIDGET_CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>
#include <QVariant>

class VsClickableLabel : public QLabel {
    Q_OBJECT

	public:
		explicit VsClickableLabel( QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags() );
		~VsClickableLabel();

	signals:
		void clicked( QVariant labelData );

	protected:
		void mousePressEvent( QMouseEvent* event );

};

#endif // VS_WIDGET_CLICKABLELABEL_H
