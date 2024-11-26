#include "pch.h"
#include "SourceVoice.hpp"

SourceVoice::SourceVoice(std::shared_ptr<AudioFile> file, IXAudio2* xaudio)
{
	if (xaudio == nullptr) return;

	xaudio->CreateSourceVoice(
		&m_sourceVoice,
		(WAVEFORMATEX*)(&file->wfx)
	);

	m_buffer = &file->buffer;
	m_sourceVoice->SubmitSourceBuffer(m_buffer);
	empty = false;
}

SourceVoice::~SourceVoice()
{
	if(empty) return;
	m_sourceVoice->DestroyVoice();
}

void SourceVoice::PlaySound(bool overwrite)
{
	if (!overwrite && IsPlaying() || empty)	return;
	HRESULT hr;
	hr = m_sourceVoice->Stop();
	hr = m_sourceVoice->FlushSourceBuffers();
	hr = m_sourceVoice->SubmitSourceBuffer(m_buffer);
	hr = m_sourceVoice->Start();
}

bool SourceVoice::IsPlaying()
{
	XAUDIO2_VOICE_STATE state;
	m_sourceVoice->GetState(&state);
	return !(state.BuffersQueued == 1 && state.SamplesPlayed == 0 || state.BuffersQueued == 0);
}
