/* Copyright (c) 2019 Maksymilian Mruszczak */


static const char esc = 0x1b; /* escape char */

/* Glyphs */
static const char glyph_delimiter[] = "\xee\x82\xb0";
static const char glyph_subdelimiter[] = "\xee\x82\xb1";
static const char glyph_prompt[] = "%";
static const char glyph_home[] = "\xef\x80\x95";
static const char glyph_user[] = "\xef\x80\x87";
static const char glyph_branch[] = "\xee\x82\xa0";

/* Colours */
static const unsigned char col_default[] = {238, 248};
static const unsigned char col_user[] = {39, 26};
static const unsigned char col_home[] = {226, 233};
static const unsigned char col_prompt[] = {160, 236, 7}; /* error, ok, font */
static const unsigned char col_git_branch[][2] = {
	{85, 22}, /* clean */
	{215, 166} /* dirty */
};
