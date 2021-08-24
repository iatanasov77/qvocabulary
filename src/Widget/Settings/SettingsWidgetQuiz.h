#ifndef WIDGET_SETTINGS_WIDGET_QUIZ_H
#define WIDGET_SETTINGS_WIDGET_QUIZ_H

#include <QWidget>

#include "AbstractSettingsWidget.h"

namespace Ui {
	class SettingsWidgetQuiz;
}

class SettingsWidgetQuiz : public AbstractSettingsWidget
{
    Q_OBJECT

	public:
		explicit SettingsWidgetQuiz( QWidget *parent = 0 );
		~SettingsWidgetQuiz();

		virtual void apply();

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
		void initExamParameters();
		void initDisplayItemColumn();
		QList<QVariant> displayItemColumns();

	    Ui::SettingsWidgetQuiz* ui;
	    QMap<QString, QVariant> quizSettings;
};

#endif // WIDGET_SETTINGS_WIDGET_QUIZ_H
