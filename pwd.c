/**
 * Copyright (c) 2019 Maksymilian Mruszczak
 * Most basic types of pwrln segments
 */

#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pwrln.h"


typedef struct Path Path;
struct Path {
	Path *next;
	char *dir;
};

static Path * path(char *);
static Path * remove_home(Path *);


Path *
path(char *str)
{
	char buf[512];
	Path *p, *c;
	int i, offs = 1;
	p = (Path *)malloc(sizeof(Path));
	p->next = NULL;
	p->dir = (char *)malloc(sizeof(char)*2);
	p->dir[0] = '/';
	p->dir[1] = '\0';
	c = p;
	for (i=0; str[offs+i] != '\0'; ++i) {
		if (str[offs+i] == '/') {
			buf[i] = '\0';
			offs += i+1;
			i = -1;
			c->next = (Path *)malloc(sizeof(Path));
			c = c->next;
			c->dir = (char *)malloc(strlen(buf)+sizeof(char));
			strcpy(c->dir, buf);
			c->next = NULL;
		} else
			buf[i] = str[offs+i];
	}
	/* tmp; in case of lack of trailing slash at the end */
	if (i > 0) {
		buf[i] = '\0';
		c->next = (Path *)malloc(sizeof(Path));
		c = c->next;
		c->next = NULL;
		c->dir = (char *)malloc(strlen(buf)+sizeof(char));
		strcpy(c->dir, buf);
	}
	return p;
}

Path *
remove_home(Path *p)
{
	Path *np, *cp, *ch, *h = path(getenv("HOME"));
	cp = p;
	for (ch=h; ch != NULL; ch=ch->next) {
		if (cp == NULL || strcmp(ch->dir, cp->dir)) {
			cp = p;
			break;
		} else
			cp = cp->next;
	}
	while (p != cp && p != NULL) {
		np = p->next;
		free(p->dir);
		free(p);
		p = np;
	}
	while (h != NULL) {
		ch = h->next;
		free(h->dir);
		free(h);
		h = ch;
	}
	return p;
}

Segment *
user(void)
{
	struct passwd *pw;
	pw = getpwuid(getuid());
	Segment *s = new(pw->pw_name, 39, 26);
	return s;
}

Segment *
pwd(void)
{
	Path *p, *c;
	Segment *s, *h = NULL;
	char buf[512], del[6];
	sprintf(del, " %s ", sdelim);
	p = path(getenv("PWD"));
	c = remove_home(p);
	if (c != p) {
		h = new("~", 226, 233);
		h->bold = 1;
		p = c;
		if (c == NULL)
			return h;
	}
	strcpy(buf, p->dir);
	for (c=p->next; c != NULL; c=c->next) {
		strcat(buf, del);
		strcat(buf, c->dir);
	}
	while (p != NULL) {
		c = p->next;
		free(p->dir);
		free(p);
		p = c;
	}
	s = new(buf, 238, 248);
	if (h != NULL)
		h->next = s;
	else
		h = s;
	return h;
}

Segment *
prompt(int status)
{
	Segment *s;
	int col = status ? 160 : 236;
	s = new(prp, col, 7);
	s->bold = 1;
	return s;
}
