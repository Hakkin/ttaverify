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
    if ((status = ttaVerify(argv[1])) != TTA_OK)
    {
        switch (status)
        {
        case TTA_BADIO:
            printf("Error: Could not read file '%s'\n", argv[1]);
            return TTA_BADIO;
        case TTA_BADMEM:
            printf("Error: Could not allocate memory for file '%s'\n", argv[1]);
            return TTA_BADMEM;
        case TTA_BADFORMAT:
            printf("Error: File '%s' is not a TTA file\n", argv[1]);
            return TTA_BADFORMAT;
        case TTA_BADHEADER:
            printf("Error: File '%s' has a corrupt header\n", argv[1]);
            return TTA_BADHEADER;
        case TTA_BADSEEK:
            printf("Error: File '%s' has a corrupt seek table\n", argv[1]);
            return TTA_BADSEEK;
        case TTA_BADFRAME:
            printf("Error: File '%s' has one or more corrupt frames\n", argv[1]);
        case TTA_OK: // To stop compiler warning
            break;
        }
    }

    return 0;
}
