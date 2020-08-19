/**
 * Copyright (c) 2019-2020 Maksymilian Mruszczak
 * pwrln git status segment
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pwrln.h"
#include "config.h"


static int
run_git(char *dst, const char *arg, size_t dsize)
{
	FILE *f;
	char cmd[TMPSIZ];
	strlcpy(cmd, "git ", TMPSIZ);
	strlcat(cmd, arg, TMPSIZ);
	strlcat(cmd, " 2>/dev/null", TMPSIZ);
	f = popen(cmd, "r");
	if (f == NULL)
		return 1;
	if (fgets(dst, dsize, f) == NULL)
		dst[0] = '\0';
	return pclose(f);
}

static int
is_dirty(void)
{
	char buf[TMPSIZ];
	if (run_git(buf, "status --porcelain", TMPSIZ))
		return 0;
	return (buf[0] != '\0');
}

Segment *
git(void)
{
	char branch[TMPSIZ];
	int dirty = is_dirty();
	if (run_git(branch, "rev-parse --abbrev-ref HEAD", TMPSIZ))
		return NULL;
	branch[strcspn(branch, "\n")] = ' ';
	strlcat(branch, glyph_branch, TMPSIZ);
	return new(branch, col_git_branch[dirty][0], col_git_branch[dirty][1]);
}
