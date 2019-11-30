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
#include "config.h"


typedef struct Path Path;
struct Path {
	Path *next;
	char *dir;
};

static Path * path(char *);
static Path * remove_home(Path *);


static Path *
path(char *str)
{
	char buf[TMPSIZ];
	Path *p, *c;
	int i, offs = 1;
	p = (Path *)malloc(sizeof(Path));
	p->next = NULL;
	if (str == NULL) {
		p->dir = (char *)malloc(sizeof(char));
		p->dir[0] = '\0';
		return p;
	}
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
			strlcpy(c->dir, buf, TMPSIZ);
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
		strlcpy(c->dir, buf, TMPSIZ);
	}
	return p;
}

static Path *
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
	Segment *s;
	pw = getpwuid(getuid());
	s = new(pw->pw_name, col_user[0], col_user[1]);
	return s;
}

Segment *
pwd(void)
{
	Path *p, *c;
	Segment *s, *h = NULL;
	char buf[TMPSIZ], del[6], *cwd;
	snprintf(del, 6, " %s ", glyph_subdelimiter);
	if (!(cwd = getenv("PWD"))) {
		getcwd(buf, TMPSIZ);
		cwd = buf;
	}
	p = path(cwd);
	c = remove_home(p);
	if (c != p) {
		h = new(glyph_home, col_home[0], col_home[1]);
		h->bold = 1;
		p = c;
		if (c == NULL)
			return h;
	}
	strlcpy(buf, p->dir, TMPSIZ);
	for (c=p->next; c != NULL; c=c->next) {
		strlcat(buf, del, TMPSIZ);
		strlcat(buf, c->dir, TMPSIZ);
	}
	while (p != NULL) {
		c = p->next;
		free(p->dir);
		free(p);
		p = c;
	}
	s = new(buf, col_default[0], col_default[1]);
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
	int col = status ? col_prompt[0] : col_prompt[1];
	s = new(glyph_prompt, col, col_prompt[2]);
	s->bold = 1;
	return s;
}
