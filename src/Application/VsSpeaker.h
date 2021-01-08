#ifndef VS_SPEAKER_H
#define VS_SPEAKER_H

#include <QTextToSpeech>

class VsSpeaker : public QObject
{
	Q_OBJECT

	public:
		VsSpeaker( QObject* parent = nullptr );
		~VsSpeaker();

		QTextToSpeech* tts();

	public slots:
		void updateTts();

	private:
		void loadTts();

		QMap<QString, QVariant> ttsSettings;
		QTextToSpeech* _tts;
};

#endif	// VS_SPEAKER_H
