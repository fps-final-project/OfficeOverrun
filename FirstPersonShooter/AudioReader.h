#pragma once
#include "pch.h"


class AudioReader
{
public:
	static HRESULT ReadWAVFile();

private:
	static HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
};