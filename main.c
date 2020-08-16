/**
 * Copyright (c) 2019-2020 Maksymilian Mruszczak
 * Assemble pwrln segment chain and print it
 */

#include <stdlib.h>

#include "pwrln.h"
#include "segments.h"

int
main(int argc, char *argv[])
{
	int status = 0;
	Segment *chain;
	set_target_shell(argv[0]);

	/* prompt layout can be configured here */
	chain = user();
	tail(chain)->next = pwd();
	tail(chain)->next = git();
	if (argc > 1)
		status = atoi(argv[1]);
	tail(chain)->next = prompt(status);
	print(chain);
	return 0;
}
