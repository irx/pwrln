/* Copyright (c) Maksymilian Mruszczak */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <pwd.h>

static const char delim[] = {0xee, 0x82, 0xb0, 0x00};
static const char sdelim[] = {0xee, 0x82, 0xb1, 0x00};
static const char prp[] = "%";


typedef struct Segment Segment;
struct Segment {
	Segment *next;
	unsigned int bg, fg;
	int bold;
	char *content;
};

typedef struct Path Path;
struct Path {
	Path *next;
	char *dir;
};


Segment *
new(const char *content, unsigned char bg, unsigned int fg)
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

void
print(Segment *s)
{
	if (s->bold)
		printf("\x1b[1m");
	printf("\x1b[48;5;%d;38;5;%dm %s ", s->bg, s->fg, s->content);
	if (s->next != NULL)
		printf("\x1b[0;48;5;%d;38;5;%dm%s", s->next->bg, s->bg, delim);
	else
		printf("\x1b[0;38;5;%dm%s\x1b[0m ", s->bg, delim);
}

Path *
path(char *str)
{
	char buf[512];
	Path *p, *c;
	int i, offs = 1;
	p = (Path *)malloc(sizeof(Path));
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
		if (strcmp(ch->dir, cp->dir) == 0)
			cp = cp->next;
		else
			break;
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
		h = new("~", 238, 248);
		p = c;
	}
	strcpy(buf, p->dir);
	for (c=p->next; c != NULL; c=c->next) {
		strcat(buf, del);
		strcat(buf, c->dir);
	}
	while (p != NULL) {
		c = p;
		free(p->dir);
		free(p);
		p = c->next;
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
	int col = status ? 160 : 236;
	return new(prp, col, 7);
}

int
main(void)
{
	Segment *cur, *one = user();
	one->next = pwd();
	one->next->next = new("%", 236, 7);
	one->next->next->bold = 1;
	tail(one)->next = pwd();
	tail(one)->next = new("attention", 226, 233);
	tail(one)->next = prompt(1);
	for (cur = one; cur != NULL; cur = cur->next)
		print(cur);
	Path *p = path(getenv("HOME"));
	/*
	for (; p != NULL; p=p->next)
		printf("%s\n", p->dir);
		*/
	return 0;
}
