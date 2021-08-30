#ifndef MULTISELECTCOMBOBOX_H
#define MULTISELECTCOMBOBOX_H

#include <QComboBox>
#include <QPaintEvent>

class MultiSelectComboBox : public QComboBox
{
    Q_OBJECT;

	public:
		MultiSelectComboBox( QWidget *widget = 0 );
		virtual ~MultiSelectComboBox();

		bool eventFilter( QObject *object, QEvent *event );
		virtual void paintEvent( QPaintEvent * );
		void setDisplayText( QString text );
		QString getDisplayText() const;

	private:
		QString m_DisplayText;
};

#endif // MULTISELECTCOMBOBOX_H
