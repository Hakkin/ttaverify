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

#include <stdio.h>
#include <stdlib.h>

#include "ttaverify.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Please specify input file.\n");
        return 0;
    }

    ttaStatus status;
    for (int i = 1; i < argc; i++)
    {
        status = ttaVerify(argv[i]);
        printf("%s: ", argv[i]);
        switch (status)
        {
            case TTA_OK:
                printf("OK\n");
                break;
            case TTA_BADIO:
                printf("Could not read file\n");
                break;
            case TTA_BADMEM:
                printf("Could not allocate memory for file\n");
                break;
            case TTA_BADFORMAT:
                printf("File is not a TTA file\n");
                break;
            case TTA_BADHEADER:
                printf("File has a corrupt header\n");
                break;
            case TTA_BADSEEK:
                printf("File has a corrupt seek table\n");
                break;
            case TTA_BADFRAME:
                printf("File has one or more corrupt frames\n");
                break;
        }
    }

    return status;
}
