#ifndef WIDGET_ARCHIVECOMPARE_H
#define WIDGET_ARCHIVECOMPARE_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QModelIndex>

namespace Ui {
	class ArchiveCompareWidget;
}

class ArchiveCompareWidget : public QWidget
{
    Q_OBJECT

	public:
		ArchiveCompareWidget( QWidget *parent );
		~ArchiveCompareWidget();

	private:
		void initModel();
		void initView();

		Ui::ArchiveCompareWidget *ui;
		QWidget* mw;
		QSqlQueryModel* pModel;
		QList<int> hideColumns;

	private slots:
		void setSelection( QModelIndex index );
		void openWord( QModelIndex index );
};

#endif // WIDGET_ARCHIVECOMPARE_H
