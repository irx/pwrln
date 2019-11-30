/**
 * Copyright (c) 2019 Maksymilian Mruszczak
 * Assemble pwrln segment chain and print it
 */

#include <stdlib.h>

#include "pwrln.h"
#include "segments.h"

int
main(int argc, char *argv[])
{
	int status = 0;
	Segment *first = user();
	tail(first)->next = pwd();
	tail(first)->next = git();
	if (argc > 1)
		status = atoi(argv[1]);
	tail(first)->next = prompt(status);
	print(first);
	return 0;
}
