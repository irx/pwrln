/**
 * Copyright (c) 2019 Maksymilian Mruszczak
 * pwrln generic segment functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pwrln.h"


Segment *
new(const char *content, unsigned char bg, unsigned char fg)
{
	Segment *s = (Segment *)malloc(sizeof(Segment));
	s->bg = bg;
	s->fg = fg;
	s->bold = 0;
	s->next = NULL;
	s->content = (char *)malloc(strlen(content)+sizeof(char));
	strcpy(s->content, content);
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
	char tmp[512], *buf = (char *)malloc(sizeof(char)*512);
	if (s->bold)
		sprintf(buf, "\001%c[1m\002", esc);
	else
		buf[0] = '\0';
	sprintf(tmp, "\001%c[48;5;%d;38;5;%dm\002 %s ", esc, s->bg, s->fg, s->content);
	strcat(buf, tmp);
	if (s->next != NULL)
		sprintf(tmp, "\001%c[0;48;5;%d;38;5;%dm\002%s", esc, s->next->bg, s->bg, delim);
	else
		sprintf(tmp, "\001%c[0;38;5;%dm\002%s\001%c[0m\002 ", esc, s->bg, delim, esc);
	strcat(buf, tmp);
	return buf;
}

void
print(Segment *s)
{
	char *tmp, buf[512];
	buf[0] = '\0';
	while (s != NULL) {
		tmp = render(s);
		strcat(buf, tmp);
		s = s->next;
		free(tmp);
	}
	printf("%s", buf);
}
