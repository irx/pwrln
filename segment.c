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
	SH_KSH,
	SH_ZSH
};

static enum Shell target_shell;
static char esc_delim[2][3];

static void print_version(void);


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
	} else if (!strncmp(sh_name, "zsh", TMPSIZ)) {
		target_shell = SH_ZSH;
		esc_delim[0][0] = '%';
		esc_delim[1][0] = '%';
		esc_delim[0][1] = '{';
		esc_delim[1][1] = '}';
		esc_delim[0][2] = 0x00;
		esc_delim[1][2] = 0x00;
	} else {
		target_shell = SH_GENERIC;
		esc_delim[0][0] = 0x00;
		esc_delim[1][0] = 0x00;
	}
}

int /* only version param for now */
parse_arg(const char *arg)
{
	if (arg[0] != '-')
		return 0;
	switch (arg[1]) {
	case '-':
		if (!strcmp(arg, "--version")) {
			print_version();
			return 1;
		}
		break;
	case 'v':
		if (arg[2] == '\0') {
			print_version();
			return 1;
		}
		break;
	}
	return 0;
}

static void
print_version(void)
{
	Segment *v = new("PWR", 226, 233);
	v->next = new("LN \xe2\x9a\xa1", 233, 226);
	v->next->next = new(VERSION, 7, 236);
	print(v);
	putchar('\n');
	prune(v);
}

Segment *
new(const char *content, unsigned char bg, unsigned char fg)
{
	Segment *s = (Segment *)malloc(sizeof(Segment));
	if (!s)
		fail("segment init error");
	s->bg = bg;
	s->fg = fg;
	s->bold = 0;
	s->next = NULL;
	s->content = (char *)malloc(strlen(content)+sizeof(char));
	if (!s->content)
		fail("segment content buffer init error");
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
	if (!buf)
		fail("segment render buffer allocation error");
	if (s->bold)
		snprintf(buf, TMPSIZ, "%s%c[1m%s", esc_delim[0], esc, esc_delim[1]);
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

void
fail(const char *msg)
{
	fprintf(stderr, "pwrln failure: %s: ", msg);
	perror(NULL);
	printf("pwrln prompt failed> ");
	exit(1);
}
