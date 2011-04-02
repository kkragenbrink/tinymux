/*! \file utf8tables.h
 * \brief Static tables with UTF-8 state machines.
 *
 * $Id$
 *
 */

#ifndef UTF8TABLES_H
#define UTF8TABLES_H

typedef struct
{
    size_t n_bytes;
    size_t n_points;
    const UTF8 *p;
} string_desc;

// utf/cl_Printable.txt
//
// 100312 included, 1013800 excluded, 0 errors.
// 198 states, 95 columns, 3622 bytes
//
#define CL_PRINT_START_STATE (0)
#define CL_PRINT_ACCEPTING_STATES_START (198)
extern const unsigned char cl_print_itt[256];
extern const unsigned short cl_print_sot[198];
extern const unsigned char cl_print_sbt[2970];

// utf/cl_AttrNameInitial.txt
//
// 177 included, 1113935 excluded, 0 errors.
// 6 states, 14 columns, 312 bytes
//
#define CL_ATTRNAMEINITIAL_START_STATE (0)
#define CL_ATTRNAMEINITIAL_ACCEPTING_STATES_START (6)
extern const unsigned char cl_attrnameinitial_itt[256];
extern const unsigned char cl_attrnameinitial_sot[6];
extern const unsigned char cl_attrnameinitial_sbt[50];

// utf/cl_AttrName.txt
//
// 203 included, 1113909 excluded, 0 errors.
// 6 states, 14 columns, 312 bytes
//
#define CL_ATTRNAME_START_STATE (0)
#define CL_ATTRNAME_ACCEPTING_STATES_START (6)
extern const unsigned char cl_attrname_itt[256];
extern const unsigned char cl_attrname_sot[6];
extern const unsigned char cl_attrname_sbt[50];

// utf/cl_ObjectName.txt
//
// 257 included, 1113855 excluded, 0 errors.
// 8 states, 23 columns, 343 bytes
//
#define CL_OBJECTNAME_START_STATE (0)
#define CL_OBJECTNAME_ACCEPTING_STATES_START (8)
extern const unsigned char cl_objectname_itt[256];
extern const unsigned char cl_objectname_sot[8];
extern const unsigned char cl_objectname_sbt[79];

// utf/cl_PlayerName.txt
//
// 190 included, 1113922 excluded, 0 errors.
// 6 states, 14 columns, 312 bytes
//
#define CL_PLAYERNAME_START_STATE (0)
#define CL_PLAYERNAME_ACCEPTING_STATES_START (6)
extern const unsigned char cl_playername_itt[256];
extern const unsigned char cl_playername_sot[6];
extern const unsigned char cl_playername_sbt[50];

// utf/cl_8859_1.txt
//
// 191 included, 1113921 excluded, 0 errors.
// 3 states, 6 columns, 279 bytes
//
#define CL_8859_1_START_STATE (0)
#define CL_8859_1_ACCEPTING_STATES_START (3)
extern const unsigned char cl_8859_1_itt[256];
extern const unsigned char cl_8859_1_sot[3];
extern const unsigned char cl_8859_1_sbt[20];

// utf/cl_8859_2.txt
//
// 191 included, 1113921 excluded, 0 errors.
// 6 states, 21 columns, 335 bytes
//
#define CL_8859_2_START_STATE (0)
#define CL_8859_2_ACCEPTING_STATES_START (6)
extern const unsigned char cl_8859_2_itt[256];
extern const unsigned char cl_8859_2_sot[6];
extern const unsigned char cl_8859_2_sbt[73];

// utf/tr_utf8_ascii.txt
//
// 2360 code points.
// 91 states, 192 columns, 3695 bytes
//
#define TR_ASCII_START_STATE (0)
#define TR_ASCII_ACCEPTING_STATES_START (91)
extern const unsigned char tr_ascii_itt[256];
extern const unsigned short tr_ascii_sot[91];
extern const unsigned char tr_ascii_sbt[3257];

// utf/tr_utf8_cp437.txt
//
// 2473 code points.
// 100 states, 193 columns, 7550 bytes
//
#define TR_CP437_START_STATE (0)
#define TR_CP437_ACCEPTING_STATES_START (100)
extern const unsigned char tr_cp437_itt[256];
extern const unsigned short tr_cp437_sot[100];
extern const unsigned short tr_cp437_sbt[3547];

// utf/tr_utf8_latin1.txt
//
// 2397 code points.
// 93 states, 192 columns, 7064 bytes
//
#define TR_LATIN1_START_STATE (0)
#define TR_LATIN1_ACCEPTING_STATES_START (93)
extern const unsigned char tr_latin1_itt[256];
extern const unsigned short tr_latin1_sot[93];
extern const unsigned short tr_latin1_sbt[3311];

// utf/tr_utf8_latin2.txt
//
// 2365 code points.
// 91 states, 192 columns, 7010 bytes
//
#define TR_LATIN2_START_STATE (0)
#define TR_LATIN2_ACCEPTING_STATES_START (91)
extern const unsigned char tr_latin2_itt[256];
extern const unsigned short tr_latin2_sot[91];
extern const unsigned short tr_latin2_sbt[3286];

// utf/tr_tolower.txt
//
// 1023 code points.
// 46 states, 86 columns, 1701 bytes
//
#define TR_TOLOWER_START_STATE (0)
#define TR_TOLOWER_ACCEPTING_STATES_START (46)
extern const unsigned char tr_tolower_itt[256];
extern const unsigned short tr_tolower_sot[46];
extern const unsigned char tr_tolower_sbt[1353];

#define TR_TOLOWER_DEFAULT (0)
#define TR_TOLOWER_LITERAL_START (1)
#define TR_TOLOWER_XOR_START (13)
extern const string_desc tr_tolower_ott[97];

// utf/tr_toupper.txt
//
// 1030 code points.
// 48 states, 90 columns, 1796 bytes
//
#define TR_TOUPPER_START_STATE (0)
#define TR_TOUPPER_ACCEPTING_STATES_START (48)
extern const unsigned char tr_toupper_itt[256];
extern const unsigned short tr_toupper_sot[48];
extern const unsigned char tr_toupper_sbt[1444];

#define TR_TOUPPER_DEFAULT (0)
#define TR_TOUPPER_LITERAL_START (1)
#define TR_TOUPPER_XOR_START (11)
extern const string_desc tr_toupper_ott[102];

// utf/tr_totitle.txt
//
// 1034 code points.
// 48 states, 90 columns, 1797 bytes
//
#define TR_TOTITLE_START_STATE (0)
#define TR_TOTITLE_ACCEPTING_STATES_START (48)
extern const unsigned char tr_totitle_itt[256];
extern const unsigned short tr_totitle_sot[48];
extern const unsigned char tr_totitle_sbt[1445];

#define TR_TOTITLE_DEFAULT (0)
#define TR_TOTITLE_LITERAL_START (1)
#define TR_TOTITLE_XOR_START (11)
extern const string_desc tr_totitle_ott[100];

// utf/tr_foldmatch.txt
//
// 14 code points.
// 7 states, 11 columns, 310 bytes
//
#define TR_FOLDMATCH_START_STATE (0)
#define TR_FOLDMATCH_ACCEPTING_STATES_START (7)
extern const unsigned char tr_foldmatch_itt[256];
extern const unsigned char tr_foldmatch_sot[7];
extern const unsigned char tr_foldmatch_sbt[47];

#define TR_FOLDMATCH_DEFAULT (0)
#define TR_FOLDMATCH_LITERAL_START (1)
#define TR_FOLDMATCH_XOR_START (3)
extern const string_desc tr_foldmatch_ott[3];

// utf/tr_Color.txt
//
// 2053 code points.
// 37 states, 67 columns, 4820 bytes
//
#define TR_COLOR_START_STATE (0)
#define TR_COLOR_ACCEPTING_STATES_START (37)
extern const unsigned char tr_color_itt[256];
extern const unsigned short tr_color_sot[37];
extern const unsigned short tr_color_sbt[2245];

#endif // UTF8TABLES_H
