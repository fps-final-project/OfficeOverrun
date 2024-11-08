#pragma once
#include "AudioFile.hpp"

#include <string>

class AudioReader
{
public:
	static AudioFile ReadWAVFile(std::string path, const UINT32 loop);

private:
	static HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
};