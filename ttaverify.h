/*
 * This software is in the public domain.
 * Where that dedication is not recognized, you are granted a perpetual,
 * irrevocable license to copy and modify these files however you want.
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied.
 * In no event shall the author(s) be liable for any claim, damages or other
 * liability arising from, out of or in connection with the software or the
 * use or other dealings in the software.
 */

#ifndef TTAVERIFY_H_INCLUDED
#define TTAVERIFY_H_INCLUDED

typedef enum
{
	TTA_OK,		// Success
	TTA_BADIO,	 // IO error (couldn't read file)
	TTA_BADMEM,	// Memory error (could't allocate memory)
	TTA_BADFORMAT, // Format error (not TTA file)
	TTA_BADHEADER, // Stream header CRC error
	TTA_BADSEEK,   // Seek table CRC error
	TTA_BADFRAME   // Frame CRC error
} ttaStatus;

ttaStatus ttaVerify(char *filename);

#endif // TTAVERIFY_H_INCLUDED
