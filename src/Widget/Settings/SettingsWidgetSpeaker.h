#ifndef WIDGET_SETTINGS_WIDGET_SPEAKER_H
#define WIDGET_SETTINGS_WIDGET_SPEAKER_H

#include <QWidget>
#include <QTextToSpeech>
#include <QStatusBar>

#include "AbstractSettingsWidget.h"

namespace Ui {
	class SettingsWidgetSpeaker;
}

class SettingsWidgetSpeaker : public AbstractSettingsWidget
{
    Q_OBJECT

	public:
		explicit SettingsWidgetSpeaker( QString title, QWidget *parent = 0 );
		~SettingsWidgetSpeaker();

		virtual void apply();

	public slots:
		void speak();
	    void stop();

	    void setRate( int );
	    void setPitch( int );
	    void setVolume( int volume );

	    void stateChanged( QTextToSpeech::State state );
		void engineSelected( int index );
		void languageSelected( int language );
		void voiceSelected( int index );

		void localeChanged( const QLocale &locale );

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
		void init();

	    Ui::SettingsWidgetSpeaker* ui;
	    QStatusBar* statusbar;
	    QTextToSpeech* speeker;
	    QVector<QVoice> voices;
	    QMap<QString, QVariant> speakerSettings;
};

#endif // WIDGET_SETTINGS_WIDGET_SPEAKER_H
