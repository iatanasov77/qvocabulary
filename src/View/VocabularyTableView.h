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

	protected:
		//void dropEvent( QDropEvent *e );
	private:
		QDialog *_popup;
		QLabel *_popupLabel;
		int m_dropRow;

		void _init();
		void _initHeader();
		void _initPopup();
		void _showPopup ( const QModelIndex &index ) const;
		int _selectedRow() const;

};

#endif // VIEW_VOCABULARYTABLEVIEW_H
