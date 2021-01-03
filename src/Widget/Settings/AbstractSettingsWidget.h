#ifndef ABSTRACT_SETTINGS_WIDGET_H
#define ABSTRACT_SETTINGS_WIDGET_H

#include <QWidget>
#include <QSettings>

class AbstractSettingsWidget : public QWidget
{
    Q_OBJECT

	protected:
		QSettings* settings;

	public:
		explicit AbstractSettingsWidget( QWidget *parent = 0 );

		virtual void apply() = 0;
};

#endif // ABSTRACT_SETTINGS_WIDGET_H
