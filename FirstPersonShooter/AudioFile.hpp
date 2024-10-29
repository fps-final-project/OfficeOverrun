#pragma once
#include <xaudio2.h>


struct AudioFile
{
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;
	std::shared_ptr<BYTE> pDataBuffer;
	IXAudio2SourceVoice* pXAudio2SourceVoice;
};