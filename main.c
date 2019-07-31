#include <stdio.h>
#include <stdlib.h>

#include "ttaverify.h"

int main(int argc, char *argv[])
{
	setbuf(stderr, NULL);

	if (argc < 2)
	{
		fprintf(stderr, "Please specify input file.\n");
		return 0;
	}

	ttaStatus status;
	for (int i = 1; i < argc; i++)
	{
		fprintf(stderr, "%s: ", argv[i]);
		status = ttaVerify(argv[i]);
		switch (status)
		{
			case TTA_OK:
				fprintf(stderr, "OK\n");
				break;
			case TTA_BADIO:
				fprintf(stderr, "Could not read file\n");
				break;
			case TTA_BADMEM:
				fprintf(stderr, "Could not allocate memory for file\n");
				break;
			case TTA_BADFORMAT:
				fprintf(stderr, "File is not a TTA file\n");
				break;
			case TTA_BADHEADER:
				fprintf(stderr, "File has a corrupt or invalid header\n");
				break;
			case TTA_BADSEEK:
				fprintf(stderr, "File has a corrupt seek table\n");
				break;
			case TTA_BADFRAME:
				fprintf(stderr, "File has one or more corrupt frames\n");
				break;
		}
	}

	return status;
}
