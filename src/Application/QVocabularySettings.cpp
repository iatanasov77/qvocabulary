#include "QVocabularySettings.h"

QVocabularySettings* QVocabularySettings::instance() {
	if ( ! _instance ) {
		_instance = new QVocabularySettings();
	}

	return static_cast<QVocabularySettings*>( _instance );
}

QMap<QString, QVariant> QVocabularySettings::speakerSettings()
{
	QMap<QString, QVariant> ttsSettings;

	ttsSettings["pitch"]	= value( "pitch", "Speaker", QVariant( 0 ) );
	ttsSettings["rate"]		= value( "rate", "Speaker", QVariant( 0 ) );
	ttsSettings["volume"]	= value( "volume", "Speaker", QVariant( 70 ) );
	ttsSettings["engine"]	= value( "engine", "Speaker", QVariant( "default" ) );
	ttsSettings["language"]	= value( "language", "Speaker", QVariant( "en_US" ) );
	ttsSettings["voice"]	= value( "voice", "Speaker", QVariant( 0 ) );

	return ttsSettings;
}

QMap<QString, QVariant> QVocabularySettings::quizSettings()
{
	QMap<QString, QVariant> quizSettings;

	quizSettings["displayTranscriptions"]	= value( "displayTranscriptions", "Quiz", QVariant( false ) );
	quizSettings["displayQuizAnswerStatus"]	= value( "displayQuizAnswerStatus", "Quiz", QVariant( false ) );
	quizSettings["randomizeWords"]			= value( "randomizeWords", "Quiz", QVariant( true ) );
	quizSettings["displayTimer"]			= value( "displayTimer", "Quiz", QVariant( false ) );
	quizSettings["timerDefaultTime"]		= value( "timerDefaultTime", "Quiz", QVariant( 1800 ) );

	return quizSettings;
}

QMap<QString, QVariant> QVocabularySettings::importSettings()
{
	QMap<QString, QVariant> importSettings;

	importSettings["groupPrefix"]	= value( "groupPrefix", "Import", QVariant( "" ) );

	return importSettings;
}
