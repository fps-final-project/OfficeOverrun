#pragma once
#include "AudioFile.hpp"

class SourceVoice
{
public:
	SourceVoice(std::shared_ptr<AudioFile> file, IXAudio2* xaudio);
	~SourceVoice();
	

	void PlaySound(bool overwrite);
	bool IsPlaying();

	inline IXAudio2SourceVoice* getSourceVoice() { return m_sourceVoice; }


private:
	IXAudio2SourceVoice* m_sourceVoice;
	XAUDIO2_BUFFER* m_buffer;
	bool empty = true;
};

