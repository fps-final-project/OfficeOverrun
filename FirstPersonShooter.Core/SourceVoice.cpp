#include "pch.h"
#include "SourceVoice.hpp"

SourceVoice::SourceVoice(std::shared_ptr<AudioFile> file, IXAudio2* xaudio) : playing(false)
{
	if (xaudio != nullptr)
	{
		xaudio->CreateSourceVoice(
			&m_sourceVoice,
			(WAVEFORMATEX*)(&file->wfx)
		);

		m_buffer = &file->buffer;
	}
	empty = !m_sourceVoice;
}

SourceVoice::~SourceVoice()
{
	if (empty) return;
	m_sourceVoice->DestroyVoice();
}

X3DAUDIO_DSP_SETTINGS SourceVoice::CalculateDSPSettings(X3DAUDIO_EMITTER* emitter, X3DAUDIO_LISTENER* listener, BYTE* x3dInstance)
{
	X3DAUDIO_DSP_SETTINGS dspSettings;
	dspSettings.DstChannelCount = 8;
	dspSettings.SrcChannelCount = 1;
	dspSettings.pMatrixCoefficients = m_matrix;

	X3DAudioCalculate(x3dInstance, listener, emitter,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER,
		&dspSettings
	);

	return dspSettings;
}

void SourceVoice::SetEmmiterSettings(X3DAUDIO_DSP_SETTINGS dspSettings, IXAudio2Voice* masteringVoice)
{
	m_sourceVoice->SetOutputMatrix(
		masteringVoice,
		dspSettings.SrcChannelCount,
		dspSettings.DstChannelCount,
		dspSettings.pMatrixCoefficients
	);
	m_sourceVoice->SetFrequencyRatio(dspSettings.DopplerFactor);
}

void SourceVoice::PlaySound(bool overwrite)
{
	if (empty || (!overwrite && IsPlaying()))	return;
	HRESULT hr;
	hr = m_sourceVoice->Stop();
	hr = m_sourceVoice->FlushSourceBuffers();
	hr = m_sourceVoice->SubmitSourceBuffer(m_buffer);
	hr = m_sourceVoice->Start();
	playing = true;
}

void SourceVoice::TogglePlay()
{
	if (empty)
		return;

	if (playing)
		m_sourceVoice->Stop();
	else m_sourceVoice->Start();

	playing = !playing;
}

bool SourceVoice::IsPlaying()
{
	if (empty)
		return false;
	XAUDIO2_VOICE_STATE state;
	m_sourceVoice->GetState(&state);
	return !(state.BuffersQueued == 0);
}
