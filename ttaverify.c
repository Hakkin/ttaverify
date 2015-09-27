#include <stdio.h>
#include <stdint.h>

#include "ttaverify.h"
#include "crc32.h"


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
static ttaStatus readTTA(ttaFile_t *file);
static int readHeader(ttaFile_t *file);
static int readSeekTable(ttaFile_t *file);
static int readFrames(ttaFile_t *file);
static int destroyTTA(ttaFIle_t *file);


ttaStatus ttaVerify(char *filename)
{
    ttaFile_t ttaFile = {0};

    if ((ttaFile.file = fopen(filename, "rb")) == NULL)
    {
        destroyTTA(ttaFile);
        return TTA_BADIO;
    }
    ttaStatus status = readTTA(&ttaFile);
    destroyTTA(ttaFile);
    return status;
}


