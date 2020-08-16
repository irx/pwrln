/**
 * Copyright (c) 2019-2020 Maksymilian Mruszczak
 * pwrln generic segment functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pwrln.h"
#include "config.h"

enum Shell {
	SH_GENERIC,
	SH_BASH,
	SH_KSH
};

static enum Shell target_shell;
static char esc_delim[2][2];


void
set_target_shell(const char *name)
{
	char buf[TMPSIZ], *word, *sh_name;
	strlcpy(buf, name, TMPSIZ);
	for (word = strtok(buf, "-"); word; word = strtok(NULL, "-"))
		sh_name = word;
	if (!strncmp(sh_name, "bash", TMPSIZ)) {
		target_shell = SH_BASH;
		esc_delim[0][0] = 0x01;
		esc_delim[1][0] = 0x02;
		esc_delim[0][1] = 0x00;
		esc_delim[1][1] = 0x00;
	} else if (!strncmp(sh_name, "ksh", TMPSIZ)) {
		target_shell = SH_KSH;
		esc_delim[0][0] = 0x01;
		esc_delim[1][0] = 0x01;
		esc_delim[0][1] = 0x00;
		esc_delim[1][1] = 0x00;
	} else {
		target_shell = SH_GENERIC;
		esc_delim[0][0] = 0x00;
		esc_delim[1][0] = 0x00;
	}
}

Segment *
new(const char *content, unsigned char bg, unsigned char fg)
{
	Segment *s = (Segment *)malloc(sizeof(Segment));
	s->bg = bg;
	s->fg = fg;
	s->bold = 0;
	s->next = NULL;
	s->content = (char *)malloc(strlen(content)+sizeof(char));
	strlcpy(s->content, content, TMPSIZ);
	return s;
}

Segment *
tail(Segment *s)
{
	while (s->next != NULL)
		s = s->next;
	return s;
}

void
prune(Segment *s)
{
	Segment *n;
	while (s != NULL) {
		n = s->next;
		free(s->content);
		free(s);
		s = n;
	}
}

char *
render(Segment *s)
{
	char tmp[TMPSIZ], *buf = (char *)malloc(sizeof(char)*512);
	if (s->bold)
		snprintf(buf, TMPSIZ, "\001%c[1m\002", esc);
	else
		buf[0] = '\0';
	snprintf(tmp, TMPSIZ, "%s%c[48;5;%d;38;5;%dm%s %s ",
	        esc_delim[0], esc, s->bg, s->fg, esc_delim[1], s->content);
	strlcat(buf, tmp, TMPSIZ);
	if (s->next != NULL)
		snprintf(tmp, TMPSIZ, "%s%c[0;48;5;%d;38;5;%dm%s%s",
		        esc_delim[0], esc, s->next->bg, s->bg, esc_delim[1], glyph_delimiter);
	else
		snprintf(tmp, TMPSIZ, "%s%c[0;38;5;%dm%s%s%s%c[0m%s ",
		        esc_delim[0], esc, s->bg, esc_delim[1],
		        glyph_delimiter, esc_delim[0], esc, esc_delim[1]);
	strlcat(buf, tmp, TMPSIZ);
	return buf;
}

void
print(Segment *s)
{
	char *tmp, buf[512];
	buf[0] = '\0';
	if (target_shell == SH_KSH)
		printf("%s\r", esc_delim[0]);
	while (s != NULL) {
		tmp = render(s);
		strlcat(buf, tmp, TMPSIZ);
		s = s->next;
		free(tmp);
	}
	printf("%s", buf);
}
