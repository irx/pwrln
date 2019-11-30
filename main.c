/**
 * Copyright (c) 2019 Maksymilian Mruszczak
 * Assemble pwrln segment chain and print it
 */

#include <stdlib.h>

#include "pwrln.h"
#include "segments.h"

int
main(void)
{
	Segment *first = user();
	tail(first)->next = pwd();
	tail(first)->next = git();
	tail(first)->next = prompt(0);
	print(first);
	return 0;
}
