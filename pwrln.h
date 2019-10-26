/* Copyright (c) 2019 Maksymilian Mruszczak */

static const char esc = 0x1b; /* escape char */
/* Glyphs */
static const char delim[] = {0xee, 0x82, 0xb0, 0x00};
static const char sdelim[] = {0xee, 0x82, 0xb1, 0x00};
static const char prp[] = "%";


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
