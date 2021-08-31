#ifndef VIEW_VOCABULARYTABLEVIEW_H
#define VIEW_VOCABULARYTABLEVIEW_H

#include <QDialog>
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QTableView>
#include <QVBoxLayout>
#include <QHeaderView>

class VocabularyTableView : public QTableView
{
    Q_OBJECT
    QDialog *popup;
    QLabel *popupLabel;

	public:
		VocabularyTableView( QWidget *parent = Q_NULLPTR );
		bool eventFilter( QObject *watched, QEvent *event );

	public slots:
		void displayWord( QVariant wordId );

	private:
		void initHeader();
		void initPopup();
		void showPopup ( const QModelIndex &index ) const;

};

#endif // VIEW_VOCABULARYTABLEVIEW_H
