/* Copyright (c) 2019-2020 Maksymilian Mruszczak */

#define TMPSIZ 512

#ifdef NEED_STRL
#undef strlcpy
#undef strlcat
size_t strlcpy(char *dst, const char *src, size_t dsize);
size_t strlcat(char *dst, const char *src, size_t dsize);
#endif

typedef struct Segment Segment;
struct Segment {
	Segment *next;
	unsigned char bg, fg;
	int bold;
	char *content;
};

/* set target shell based on executable name */
void set_target_shell(const char *);

/* recognise param in given arg; returns 1 if succesful */
int parse_arg(const char *);

/* create new segment <content, bg col, font col> */
Segment * new(const char *, unsigned char, unsigned char);

/* get last segment in chain */
Segment * tail(Segment *);

/* destroy all consecutive segments */
void prune(Segment *);

/* generate string for single segment */
char * render(Segment *);

/* print all segments in chain into stdout */
void print(Segment *);

/* print error and exit */
void fail(const char *);
