#ifndef VS_WIDGET_MULTISELECTCOMBOBOX_H
#define VS_WIDGET_MULTISELECTCOMBOBOX_H

#include <QComboBox>
#include <QPaintEvent>

class VsMultiSelectComboBox : public QComboBox
{
    Q_OBJECT;

	public:
		VsMultiSelectComboBox( QWidget *widget = 0 );
		virtual ~VsMultiSelectComboBox();

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

#endif // VS_WIDGET_MULTISELECTCOMBOBOX_H
