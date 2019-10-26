/* Copyright (c) 2019 Maksymilian Mruszczak */


typedef struct Segment Segment;
struct Segment {
	Segment *next;
	unsigned char bg, fg;
	int bold;
	char *content;
};

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
