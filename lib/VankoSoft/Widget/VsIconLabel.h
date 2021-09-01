#ifndef VS_WIDGET_ICONLABEL_H
#define VS_WIDGET_ICONLABEL_H

#include <QWidget>

/**
 * EXAMPLE: https://stackoverflow.com/questions/10533838/displaying-a-standard-icon-and-text-in-qlabel
 * In the example is used QtAwesome : https://github.com/gamecreature/QtAwesome
 */
class VsIconLabel : public QWidget {
    Q_OBJECT

	public:
		explicit VsIconLabel( QString iconPath, QString text, bool finalStretch = true, QWidget* parent = Q_NULLPTR );
		~VsIconLabel();
};

#endif // VS_WIDGET_ICONLABEL_H
