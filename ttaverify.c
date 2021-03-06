#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "ttaverify.h"
#include "crc32.h"

#define FRAME_TIME 1.04489795918367346939

// TTA header and file structures declarations
#pragma pack(push,1)
// TTA1 format reference: http://en.true-audio.com/TTA_Lossless_Audio_Codec_-_Format_Description
typedef struct
{
	uint16_t *data;
	uint32_t crc32;
} ttaFrame_t;

typedef struct
{
	uint32_t frameSize;
} seekPoint_t;

typedef struct
{
	seekPoint_t *seekPoints;
	uint32_t crc32;
} seekTable_t;

typedef struct
{
	uint8_t  signature[4];
	uint16_t format;
	uint16_t channels;
	uint16_t bitsPerSample;
	uint32_t sampleRate;
	uint32_t dataLength;
	uint32_t crc32;
} streamHeader_t;

typedef struct
{
	FILE *file;
	streamHeader_t streamHeader;
	seekTable_t seekTable;
	ttaFrame_t *frameTable;
} ttaFile_t;
#pragma pack(pop)


// Internal functions
static ttaStatus readTTA(ttaFile_t *pFile);
static ttaStatus readHeader(ttaFile_t *pFile);
static size_t getFrameCount(ttaFile_t *pFile);
static ttaStatus readSeekTable(ttaFile_t *pFile);
static ttaStatus readFrames(ttaFile_t *pFile);
static void destroyTTA(ttaFile_t *pFile);


ttaStatus ttaVerify(char *filename)
{
	ttaFile_t ttaFile = {0};

	if ((ttaFile.file = fopen(filename, "rb")) == NULL)
	{
		destroyTTA(&ttaFile);
		return TTA_BADIO;
	}
	ttaStatus status = readTTA(&ttaFile);
	destroyTTA(&ttaFile);
	return status;
}

ttaStatus readTTA(ttaFile_t *pFile)
{
	ttaStatus status = TTA_OK;
	if ((status = readHeader(pFile)) != TTA_OK)
	{
		return status;
	}
	if ((status = readSeekTable(pFile)) != TTA_OK)
	{
		return status;
	}
	if ((status = readFrames(pFile)) != TTA_OK)
	{
		return status;
	}

	return status;
}

ttaStatus readHeader(ttaFile_t *pFile)
{
	if (fread(&pFile->streamHeader, 22, 1, pFile->file) != 1)
	{
		return TTA_BADIO;
	}
	if (pFile->streamHeader.signature[0] != 'T' ||
	    pFile->streamHeader.signature[1] != 'T' ||
	    pFile->streamHeader.signature[2] != 'A' ||
	    pFile->streamHeader.signature[3] != '1')
	{
		return TTA_BADFORMAT;
	}
	// Either of these being 0 will lead to a divide by 0 error in getFrameCount
	if (pFile->streamHeader.sampleRate == 0 ||
	    pFile->streamHeader.dataLength == 0)
	{
		return TTA_BADHEADER;
	}
	if (crc32(&pFile->streamHeader, 18) != pFile->streamHeader.crc32)
	{
		return TTA_BADHEADER;
	}

	return TTA_OK;
}

size_t getFrameCount(ttaFile_t *pFile)
{
	uint64_t samplesPerFrame = (uint64_t)(FRAME_TIME * pFile->streamHeader.sampleRate);
	uint32_t dataLength = pFile->streamHeader.dataLength;
	return dataLength / samplesPerFrame + (dataLength % samplesPerFrame ? 1 : 0);
}

ttaStatus readSeekTable(ttaFile_t *pFile)
{
	size_t frameCount = getFrameCount(pFile);

	if ((pFile->seekTable.seekPoints = malloc(4 * frameCount)) == NULL)
	{
		return TTA_BADMEM;
	}
	if (fread(pFile->seekTable.seekPoints, 4, frameCount, pFile->file) != frameCount ||
	    fread(&pFile->seekTable.crc32, 4, 1, pFile->file) != 1)
	{
		return TTA_BADIO;
	}
	if (crc32(pFile->seekTable.seekPoints, 4 * frameCount) != pFile->seekTable.crc32)
	{
		return TTA_BADSEEK;
	}

	return TTA_OK;
}

ttaStatus readFrames(ttaFile_t *pFile)
{
	size_t frameCount = getFrameCount(pFile);

	if ((pFile->frameTable = calloc(frameCount, sizeof(ttaFrame_t))) == NULL)
	{
		return TTA_BADMEM;
	}
	for (int i = 0; i < frameCount; i++)
	{
		size_t frameSize = pFile->seekTable.seekPoints[i].frameSize - 4; // -4 to compensate for CRC size
		if ((pFile->frameTable[i].data = malloc(frameSize)) == NULL)
		{
			return TTA_BADMEM;
		}
		if (fread(pFile->frameTable[i].data, 1, frameSize, pFile->file) != frameSize ||
		    fread(&pFile->frameTable[i].crc32, 4, 1, pFile->file) != 1)
		{
			return TTA_BADIO;
		}
		if (crc32(pFile->frameTable[i].data, frameSize) != pFile->frameTable[i].crc32)
		{
			return TTA_BADFRAME;
		}

		free(pFile->frameTable[i].data);
		pFile->frameTable[i].data = NULL;
	}

	return TTA_OK;
}

void destroyTTA(ttaFile_t *pFile)
{
	if (pFile->seekTable.seekPoints != NULL)
	{
		free(pFile->seekTable.seekPoints);
	}
	if (pFile->frameTable != NULL)
	{
		int frameCount = getFrameCount(pFile);
		for (int i = 0; i < frameCount; i++)
		{
			if (pFile->frameTable[i].data != NULL)
			{
				free(pFile->frameTable[i].data);
			}
		}
		free(pFile->frameTable);
	}
	if (pFile->file != NULL)
	{
		fclose(pFile->file);
	}
}