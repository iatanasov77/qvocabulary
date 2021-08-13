#ifndef VIEW_ARCHIVECOMPARE_TABLEVIEW_H
#define VIEW_ARCHIVECOMPARE_TABLEVIEW_H

#include <QDialog>
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QTableView>
#include <QVBoxLayout>
#include <QHeaderView>

class ArchiveCompareTableView : public QTableView
{
    Q_OBJECT
    QDialog *popup;
    QLabel *popupLabel;

	public:
    	ArchiveCompareTableView( QWidget *parent = Q_NULLPTR );
		bool eventFilter( QObject *watched, QEvent *event );

	private:
		void showPopup ( const QModelIndex &index, QString popupText ) const;
};

#endif // VIEW_ARCHIVECOMPARE_TABLEVIEW_H
