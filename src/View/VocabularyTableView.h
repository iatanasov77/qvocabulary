#ifndef VIEW_VOCABULARYTABLEVIEW_H
#define VIEW_VOCABULARYTABLEVIEW_H

#include <QDialog>
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QTableView>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QDropEvent>

class VocabularyTableView : public QTableView
{
    Q_OBJECT

	public:
		VocabularyTableView( QWidget *parent = Q_NULLPTR );
		bool eventFilter( QObject *watched, QEvent *event );

	public slots:
		void displayWord( QVariant wordId );

	private:
		QDialog *_popup;
		QLabel *_popupLabel;

		void _init();
		void _initHeader();
		void _initPopup();
		void _showPopup ( const QModelIndex &index ) const;
};

#endif // VIEW_VOCABULARYTABLEVIEW_H
