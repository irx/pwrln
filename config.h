/* Copyright (c) 2019-2020 Maksymilian Mruszczak */


static const char esc = 0x1b; /* escape char */

/* Glyphs */
static const char glyph_delimiter[] = "\xee\x82\xb0";
static const char glyph_subdelimiter[] = "\xee\x82\xb1";
static const char glyph_prompt[] = "\xef\xa3\xaf"; /* alt "\xef\x8a\x95"; */
static const char glyph_home[] = "\xef\x80\x95";
static const char glyph_user[] = "\xef\x80\x87";
static const char glyph_branch[] = "\xee\x82\xa0";

/* Fallback Glyphs */
/* Use those if your font lacks fancy chars.
static const char glyph_delimiter[] = "";
static const char glyph_subdelimiter[] = ">";
static const char glyph_prompt[] = "%";
static const char glyph_home[] = "~";
static const char glyph_user[] = "(u)";
static const char glyph_branch[] = "(b)";
*/

/* Colours */
static const unsigned char col_default[] = {238, 248};
static const unsigned char col_user[] = {39, 26};
static const unsigned char col_home[] = {226, 233};
static const unsigned char col_prompt[] = {160, 236, 7}; /* error, ok, font */
static const unsigned char col_git_branch[][2] = {
	{85, 22}, /* clean */
	{215, 166} /* dirty */
};
