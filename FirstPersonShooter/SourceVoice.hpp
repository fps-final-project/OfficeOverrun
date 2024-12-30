#pragma once
#include "AudioFile.hpp"
#include <x3daudio.h>


class __declspec(dllexport) SourceVoice
{
public:
	SourceVoice(std::shared_ptr<AudioFile> file, IXAudio2* xaudio);
	~SourceVoice();
	
	X3DAUDIO_DSP_SETTINGS CalculateDSPSettings(X3DAUDIO_EMITTER* emitter, X3DAUDIO_LISTENER* listener, BYTE* x3dInstance);
	void SetEmmiterSettings(X3DAUDIO_DSP_SETTINGS dspSettings, IXAudio2Voice* masteringVoice);
	void PlaySound(bool overwrite);
	void TogglePlay();
	bool IsPlaying();

	inline IXAudio2SourceVoice* GetSourceVoice() { return m_sourceVoice; }
	inline void SetVolume(float volume) { m_sourceVoice->SetVolume(volume); }



private:
	IXAudio2SourceVoice* m_sourceVoice;
	XAUDIO2_BUFFER* m_buffer;
	bool empty = true;
	bool playing = false;
	FLOAT32 m_matrix[8];
};

