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
	Segment *chain;
	int status = 0;
	if (argc > 1) {
		if (parse_arg(argv[1]))
			return 0;
		else
			status = atoi(argv[1]);
	}
	set_target_shell(argv[0]);

	/* prompt layout can be configured here */
	chain = user();
	tail(chain)->next = pwd();
	tail(chain)->next = git();
	tail(chain)->next = prompt(status);
	print(chain);
	return 0;
}
