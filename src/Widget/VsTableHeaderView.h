#ifndef VS_TABLEHEADERVIEW_H
#define VS_TABLEHEADERVIEW_H

#include <QtWidgets>
#include <QList>

class VsTableHeaderView : public QHeaderView
{
    Q_OBJECT

	public:
		explicit VsTableHeaderView( Qt::Orientation orientation, QWidget * parent = nullptr, QList<int> checkboxColumns = QList<int>() );

	protected:
		virtual void paintSection( QPainter *painter, const QRect &rect, int logicalIndex ) const;
		virtual void mousePressEvent( QMouseEvent *event );

	private:
		QMap<int, bool> isOn;
		QList<int> _checkboxColumns;
		static QMap<int, QRect> _checkboxRects;

		void initStyle();

	signals:
		void checkboxClicked( int logicalIndex, bool isOn );

	public slots:
		void checkAll( int logicalIndex, bool isOn );
};

#endif // VS_TABLEHEADERVIEW_H
