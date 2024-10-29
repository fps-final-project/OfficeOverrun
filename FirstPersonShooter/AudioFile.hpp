#pragma once
#include <xaudio2.h>


struct AudioFile
{
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;
};