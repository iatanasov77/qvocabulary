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

		void setEditable( bool editable );

	private:
		QString m_DisplayText;
		QLineEdit *_lineEdit;

	private slots:
		void onItemPressed ( const QModelIndex &index );
		void setFilterText( const QString &text );
};

#endif // MULTISELECTCOMBOBOX_H
