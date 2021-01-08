#include "VsSpeaker.h"

#include <QObject>
#include <QDebug>
#include <QTextToSpeech>

#include "Application/VsSettings.h"

VsSpeaker::VsSpeaker( QObject* parent ) : QObject( parent )
{
	loadTts();
}

VsSpeaker::~VsSpeaker()
{
	delete _tts;
}

QTextToSpeech* VsSpeaker::tts()
{
	return _tts;
}

void VsSpeaker::loadTts()
{
	ttsSettings	= VsSettings::instance()->speakerSettings();

	QString engineName	= ttsSettings["engine"].toString();

	if ( engineName == "default" )
		_tts	= new QTextToSpeech( this );
	else
		_tts	= new QTextToSpeech( engineName, this );

	QVector<QVoice> voices = _tts->availableVoices();

	_tts->setRate( ttsSettings["rate"].toInt() / 10.0 );
	_tts->setPitch( ttsSettings["pitch"].toInt() / 10.0 );
	_tts->setVolume( ttsSettings["volume"].toInt() / 100.0 );

	_tts->setLocale( ttsSettings["language"].toLocale() );
	_tts->setVoice( voices.at( ttsSettings["voice"].toInt() ) );
}

void VsSpeaker::updateTts()
{
	delete _tts;
	loadTts();
}
