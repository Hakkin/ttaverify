#ifndef TTAVERIFY_H_INCLUDED
#define TTAVERIFY_H_INCLUDED

typedef enum
{
    TTA_OK,        // Success
    TTA_BADIO,     // IO error (couldn't read file)
    TTA_BADFORMAT, // Format error (not TTA file)
    TTA_BADHEADER, // Stream header CRC error
    TTA_BADSEEK,   // Seek table CRC error
    TTA_BADFRAME   // Frame CRC error
} ttaStatus;

ttaStatus ttaVerify(char *filename);

#endif // TTAVERIFY_H_INCLUDED
