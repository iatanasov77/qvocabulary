#ifndef DIALOG_TEST_WIDGETS_H
#define DIALOG_TEST_WIDGETS_H

#include <QDialog>

namespace Ui {
	class TestWidgetsDialog;
}

class TestWidgetsDialog : public QDialog
{
    Q_OBJECT

	public:
		explicit TestWidgetsDialog( QWidget *parent = 0 );
		~TestWidgetsDialog();

	private:
		QWidget *mw;	// parent MainWindow
	    Ui::TestWidgetsDialog *ui;

	    void testWidgets();
	    void testIconLabel();
	    void testIconWidgetsDelegate();

	private slots:
		void execSomething();
		void closeDialog();
};

#endif // DIALOG_TEST_WIDGETS_H
