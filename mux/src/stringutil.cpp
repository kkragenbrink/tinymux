/*! \file stringutil.cpp
 * \brief String utility functions.
 *
 * $Id$
 *
 */

#include "copyright.h"
#include "autoconf.h"
#include "config.h"
#include "externs.h"

#include "ansi.h"
#include "pcre.h"
#include "mathutil.h"

const bool mux_isprint_ascii[256] =
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
//
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 2
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 3
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 4
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 5
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 6
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,  // 7

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // E
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   // F
};

const bool mux_isprint_latin1[256] =
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
//
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 2
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 3
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 4
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 5
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 6
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,  // 7

    0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0,  // 8
    0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1,  // 9
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // A
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // B
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // C
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // D
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // E
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1   // F
};

const bool mux_isdigit[256] =
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
//
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 2
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,  // 3
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 4
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 5
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 6
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 7

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // E
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   // F
};

const bool mux_isxdigit[256] =
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
//
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 2
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,  // 3
    0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 4
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 5
    0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 6
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 7

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // E
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   // F
};

const bool mux_isazAZ[256] =
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
//
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 2
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 3
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 4
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,  // 5
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 6
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,  // 7

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // E
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   // F
};

const bool mux_isalnum[256] =
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
//
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 2
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,  // 3
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 4
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,  // 5
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 6
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,  // 7

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // E
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   // F
};

const bool mux_isupper_ascii[256] =
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
//
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 2
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 3
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 4
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,  // 5
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 6
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 7

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // E
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   // F
};

const bool mux_islower_ascii[256] =
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
//
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 2
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 3
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 4
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 5
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 6
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,  // 7

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // E
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   // F
};

const bool mux_isspace[256] =
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
//
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,  // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 2
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 3
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 4
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 5
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 6
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 7

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // E
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   // F
};

// Characters which should be escaped for the secure()
// function: '%$\[](){},;'.
//
const bool mux_issecure[256] =
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
//
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1
    0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0,  // 2
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,  // 3
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 4
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0,  // 5
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 6
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0,  // 7

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // E
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   // F
};

// Characters which should be escaped for the escape()
// function: '%\[]{};,()^$'.
//
const bool mux_isescape[256] =
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
//
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1
    0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0,  // 2
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,  // 3
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 4
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,  // 5
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 6
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0,  // 7

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // E
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   // F
};

const bool ANSI_TokenTerminatorTable[256] =
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
//
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 2
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 3
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 4
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,  // 5
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 6
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,  // 7

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // E
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   // F
};

const unsigned char mux_hex2dec[256] =
{
//  0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
//
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 0
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 1
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 2
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  0,  0,  0,  0,  0,  0,  // 3
    0, 10, 11, 12, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 4
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 5
    0, 10, 11, 12, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 6
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 7

    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 8
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // A
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // B
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // C
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // D
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // E
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0   // F
};

const unsigned char mux_toupper_ascii[UCHAR_MAX+1] =
{
//   0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
//
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, // 0
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, // 1
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, // 2
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, // 3
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, // 4
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, // 5
    0x60, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, // 6
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, // 7

    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, // 8
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, // 9
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, // A
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, // B
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, // C
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, // D
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, // E
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF  // F
};

const unsigned char mux_tolower_ascii[UCHAR_MAX+1] =
{
//   0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
//
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, // 0
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, // 1
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, // 2
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, // 3
    0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, // 4
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, // 5
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, // 6
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, // 7

    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, // 8
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, // 9
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, // A
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, // B
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, // C
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, // D
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, // E
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF  // F
};

// This will help decode UTF-8 sequences.
//
// 0xxxxxxx ==> 00000000-01111111 ==> 00-7F 1 byte sequence.
// 10xxxxxx ==> 10000000-10111111 ==> 80-BF continue
// 110xxxxx ==> 11000000-11011111 ==> C0-DF 2 byte sequence.
// 1110xxxx ==> 11100000-11101111 ==> E0-EF 3 byte sequence.
// 11110xxx ==> 11110000-11110111 ==> F0-F7 4 byte sequence.
//              11111000-11111111 illegal
//
// Also, RFC 3629 specifies that 0xC0, 0xC1, and 0xF5-0xFF never
// appear in a valid sequence.
//
// The first byte gives the length of a sequence (UTF8_SIZE1 - UTF8_SIZE4).
// Bytes in the middle of a sequence map to UTF8_CONTINUE.  Bytes which should
// not appear map to UTF8_ILLEGAL.
//
const unsigned char utf8_FirstByte[256] =
{
//  0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
//
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 0
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 2
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 3
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7

    5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  // 8
    5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  // 9
    5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  // A
    5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  // B
    6,  6,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  // C
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  // D
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  // E
    4,  4,  4,  4,  4,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6   // F
};

// The following table maps existing 8-bit characters to their corresponding
// UTF8 sequences.
//
const UTF8 *latin1_utf8[256] =
{
   T(""),             T("\x01"),         T("\x02"),         T("\x03"),
   T("\x04"),         T("\x05"),         T("\x06"),         T("\x07"),
   T("\x08"),         T("\x09"),         T("\x0A"),         T("\x0B"),
   T("\x0C"),         T("\x0D"),         T("\x0E"),         T("\x0F"),
   T("\x10"),         T("\x11"),         T("\x12"),         T("\x13"),
   T("\x14"),         T("\x15"),         T("\x16"),         T("\x17"),
   T("\x18"),         T("\x19"),         T("\x1A"),         T("\x1B"),
   T("\x1C"),         T("\x1D"),         T("\x1E"),         T("\x1F"),
   T("\x20"),         T("\x21"),         T("\x22"),         T("\x23"),
   T("\x24"),         T("\x25"),         T("\x26"),         T("\x27"),
   T("\x28"),         T("\x29"),         T("\x2A"),         T("\x2B"),
   T("\x2C"),         T("\x2D"),         T("\x2E"),         T("\x2F"),
   T("\x30"),         T("\x31"),         T("\x32"),         T("\x33"),
   T("\x34"),         T("\x35"),         T("\x36"),         T("\x37"),
   T("\x38"),         T("\x39"),         T("\x3A"),         T("\x3B"),
   T("\x3C"),         T("\x3D"),         T("\x3E"),         T("\x3F"),
   T("\x40"),         T("\x41"),         T("\x42"),         T("\x43"),
   T("\x44"),         T("\x45"),         T("\x46"),         T("\x47"),
   T("\x48"),         T("\x49"),         T("\x4A"),         T("\x4B"),
   T("\x4C"),         T("\x4D"),         T("\x4E"),         T("\x4F"),
   T("\x50"),         T("\x51"),         T("\x52"),         T("\x53"),
   T("\x54"),         T("\x55"),         T("\x56"),         T("\x57"),
   T("\x58"),         T("\x59"),         T("\x5A"),         T("\x5B"),
   T("\x5C"),         T("\x5D"),         T("\x5E"),         T("\x5F"),
   T("\x60"),         T("\x61"),         T("\x62"),         T("\x63"),
   T("\x64"),         T("\x65"),         T("\x66"),         T("\x67"),
   T("\x68"),         T("\x69"),         T("\x6A"),         T("\x6B"),
   T("\x6C"),         T("\x6D"),         T("\x6E"),         T("\x6F"),
   T("\x70"),         T("\x71"),         T("\x72"),         T("\x73"),
   T("\x74"),         T("\x75"),         T("\x76"),         T("\x77"),
   T("\x78"),         T("\x79"),         T("\x7A"),         T("\x7B"),
   T("\x7C"),         T("\x7D"),         T("\x7E"),         T("\x7F"),
   T("\xE2\x82\xAC"), T("\xEF\xBF\xBD"), T("\xE2\x80\x9A"), T("\xC6\x92"),
   T("\xE2\x80\x9E"), T("\xE2\x80\xA6"), T("\xE2\x80\xA0"), T("\xE2\x80\xA1"),
   T("\xCB\x86"),     T("\xE2\x80\xB0"), T("\xC5\xA0"),     T("\xE2\x80\xB9"),
   T("\xC5\x92"),     T("\xEF\xBF\xBD"), T("\xC5\xBD"),     T("\xEF\xBF\xBD"),
   T("\xEF\xBF\xBD"), T("\xE2\x80\x98"), T("\xE2\x80\x99"), T("\xE2\x80\x9C"),
   T("\xE2\x80\x9D"), T("\xE2\x80\xA2"), T("\xE2\x80\x93"), T("\xE2\x80\x94"),
   T("\xCB\x9C"),     T("\xE2\x84\xA2"), T("\xC5\xA1"),     T("\xE2\x80\xBA"),
   T("\xC5\x93"),     T("\xEF\xBF\xBD"), T("\xC5\xBE"),     T("\xC5\xB8"),
   T("\xC2\xA0"),     T("\xC2\xA1"),     T("\xC2\xA2"),     T("\xC2\xA3"),
   T("\xC2\xA4"),     T("\xC2\xA5"),     T("\xC2\xA6"),     T("\xC2\xA7"),
   T("\xC2\xA8"),     T("\xC2\xA9"),     T("\xC2\xAA"),     T("\xC2\xAB"),
   T("\xC2\xAC"),     T("\xC2\xAD"),     T("\xC2\xAE"),     T("\xC2\xAF"),
   T("\xC2\xB0"),     T("\xC2\xB1"),     T("\xC2\xB2"),     T("\xC2\xB3"),
   T("\xC2\xB4"),     T("\xC2\xB5"),     T("\xC2\xB6"),     T("\xC2\xB7"),
   T("\xC2\xB8"),     T("\xC2\xB9"),     T("\xC2\xBA"),     T("\xC2\xBB"),
   T("\xC2\xBC"),     T("\xC2\xBD"),     T("\xC2\xBE"),     T("\xC2\xBF"),
   T("\xC3\x80"),     T("\xC3\x81"),     T("\xC3\x82"),     T("\xC3\x83"),
   T("\xC3\x84"),     T("\xC3\x85"),     T("\xC3\x86"),     T("\xC3\x87"),
   T("\xC3\x88"),     T("\xC3\x89"),     T("\xC3\x8A"),     T("\xC3\x8B"),
   T("\xC3\x8C"),     T("\xC3\x8D"),     T("\xC3\x8E"),     T("\xC3\x8F"),
   T("\xC3\x90"),     T("\xC3\x91"),     T("\xC3\x92"),     T("\xC3\x93"),
   T("\xC3\x94"),     T("\xC3\x95"),     T("\xC3\x96"),     T("\xC3\x97"),
   T("\xC3\x98"),     T("\xC3\x99"),     T("\xC3\x9A"),     T("\xC3\x9B"),
   T("\xC3\x9C"),     T("\xC3\x9D"),     T("\xC3\x9E"),     T("\xC3\x9F"),
   T("\xC3\xA0"),     T("\xC3\xA1"),     T("\xC3\xA2"),     T("\xC3\xA3"),
   T("\xC3\xA4"),     T("\xC3\xA5"),     T("\xC3\xA6"),     T("\xC3\xA7"),
   T("\xC3\xA8"),     T("\xC3\xA9"),     T("\xC3\xAA"),     T("\xC3\xAB"),
   T("\xC3\xAC"),     T("\xC3\xAD"),     T("\xC3\xAE"),     T("\xC3\xAF"),
   T("\xC3\xB0"),     T("\xC3\xB1"),     T("\xC3\xB2"),     T("\xC3\xB3"),
   T("\xC3\xB4"),     T("\xC3\xB5"),     T("\xC3\xB6"),     T("\xC3\xB7"),
   T("\xC3\xB8"),     T("\xC3\xB9"),     T("\xC3\xBA"),     T("\xC3\xBB"),
   T("\xC3\xBC"),     T("\xC3\xBD"),     T("\xC3\xBE"),     T("\xC3\xBF"),
};

/*! \brief Validates UTF8 string and returns number of code points contained therein.
 *
 * \param pString   UTF8 string.
 * \param nString   Resulting number of code points in pString.
 * \return          true for valid, false for invalid.
 */

bool utf8_strlen(const UTF8 *pString, size_t &nString)
{
    nString = 0;
    int i = 0;
    while ('\0' != pString[i])
    {
        unsigned char t = utf8_FirstByte[pString[i]];
        if (UTF8_CONTINUE <= t)
        {
            return false;
        }

        int j;
        for (j = i + 1; j < i + t; j++)
        {
            if (  '\0' == pString[j]
               || UTF8_CONTINUE != utf8_FirstByte[pString[j]])
            {
                return false;
            }
        }
        nString++;
        i = i + t;
    }
    return true;
}

bool utf8_strlen(const UTF8 *pString, mux_cursor &nString)
{
    LBUF_OFFSET nPoints = 0;
    LBUF_OFFSET nBytes  = 0;
    while ('\0' != pString[nBytes])
    {
        unsigned char t = utf8_FirstByte[pString[nBytes]];
        if (UTF8_CONTINUE <= t)
        {
            nString(nBytes, nPoints);
            return false;
        }

        int j;
        for (j = nBytes + 1; j < nBytes + t; j++)
        {
            if (  '\0' == pString[j]
               || UTF8_CONTINUE != utf8_FirstByte[pString[j]])
            {
                nString(nBytes, nPoints);
                return false;
            }
        }
        nPoints++;
        nBytes = nBytes + t;
    }
    nString(nBytes, nPoints);
    return true;
}

const int g_trimoffset[4][4] =
{
    { 0, 1, 1, 1 },
    { 1, 0, 2, 2 },
    { 2, 1, 0, 3 },
    { 3, 2, 1, 0 }
};

/*! \brief Convert UTF8 to latin1 with '?' for all unsupported characters.
 *
 * \param pString   UTF8 string.
 * \return          Equivalent string in latin1 codeset.
 */

const char *ConvertToLatin(const UTF8 *pString)
{
    static char buffer[LBUF_SIZE];
    char *q = buffer;

    while ('\0' != *pString)
    {
        const UTF8 *p = pString;
        int iState = TR_LATIN1_START_STATE;
        do
        {
            unsigned char ch = *p++;
            iState = tr_latin1_stt[iState][tr_latin1_itt[(unsigned char)ch]];
        } while (iState < TR_LATIN1_ACCEPTING_STATES_START);
        *q++ = (char)(iState - TR_LATIN1_ACCEPTING_STATES_START);
        pString = utf8_NextCodePoint(pString);
    }
    *q = '\0';
    return buffer;
}

/*! \brief Convert UTF8 to ASCII with '?' for all unsupported characters.
 *
 * \param pString   UTF8 string.
 * \return          Equivalent string in ASCII codeset.
 */

const char *ConvertToAscii(const UTF8 *pString)
{
    static char buffer[LBUF_SIZE];
    char *q = buffer;

    while ('\0' != *pString)
    {
        const UTF8 *p = pString;
        int iState = TR_ASCII_START_STATE;
        do
        {
            unsigned char ch = *p++;
            iState = tr_ascii_stt[iState][tr_ascii_itt[(unsigned char)ch]];
        } while (iState < TR_ASCII_ACCEPTING_STATES_START);
        *q++ = (char)(iState - TR_ASCII_ACCEPTING_STATES_START);
        pString = utf8_NextCodePoint(pString);
    }
    *q = '\0';
    return buffer;
}

#define CS_FOREGROUND ((UINT16)0x000F)
#define CS_FG_BLACK   ((UINT16)0x0000)
#define CS_FG_RED     ((UINT16)0x0001)
#define CS_FG_GREEN   ((UINT16)0x0002)
#define CS_FG_YELLOW  ((UINT16)0x0003)
#define CS_FG_BLUE    ((UINT16)0x0004)
#define CS_FG_MAGENTA ((UINT16)0x0005)
#define CS_FG_CYAN    ((UINT16)0x0006)
#define CS_FG_WHITE   ((UINT16)0x0007)
#define CS_FG_DEFAULT ((UINT16)0x0008)
#define CS_BACKGROUND ((UINT16)0x00F0)
#define CS_BG_BLACK   ((UINT16)0x0000)
#define CS_BG_RED     ((UINT16)0x0010)
#define CS_BG_GREEN   ((UINT16)0x0020)
#define CS_BG_YELLOW  ((UINT16)0x0030)
#define CS_BG_BLUE    ((UINT16)0x0040)
#define CS_BG_MAGENTA ((UINT16)0x0050)
#define CS_BG_CYAN    ((UINT16)0x0060)
#define CS_BG_WHITE   ((UINT16)0x0070)
#define CS_BG_DEFAULT ((UINT16)0x0080)
#define CS_INTENSE    ((UINT16)0x0100)
#define CS_INVERSE    ((UINT16)0x0200)
#define CS_UNDERLINE  ((UINT16)0x0400)
#define CS_BLINK      ((UINT16)0x0800)
#define CS_ATTRS      ((UINT16)0x0F00)
#define CS_ALLBITS    ((UINT16)0xFFFF)

#define CS_NORMAL     (CS_FG_DEFAULT|CS_BG_DEFAULT)
#define CS_NOBLEED    (CS_FG_WHITE|CS_BG_DEFAULT)

const MUX_COLOR_SET aColors[COLOR_LAST_CODE+1] =
{
    { 0,             0,             "",            0,                       T(""),               0, T(""),    0}, // COLOR_NOTCOLOR
    { CS_NORMAL,     CS_ALLBITS,    ANSI_NORMAL,   sizeof(ANSI_NORMAL)-1,   T(COLOR_RESET),      3, T("%xn"), 3}, // COLOR_INDEX_RESET
    { CS_INTENSE,    CS_INTENSE,    ANSI_HILITE,   sizeof(ANSI_HILITE)-1,   T(COLOR_INTENSE),    3, T("%xh"), 3}, // COLOR_INDEX_ATTR, COLOR_INDEX_INTENSE
    { CS_UNDERLINE,  CS_UNDERLINE,  ANSI_UNDER,    sizeof(ANSI_UNDER)-1,    T(COLOR_UNDERLINE),  3, T("%xu"), 3}, // COLOR_INDEX_UNDERLINE
    { CS_BLINK,      CS_BLINK,      ANSI_BLINK,    sizeof(ANSI_BLINK)-1,    T(COLOR_BLINK),      3, T("%xf"), 3}, // COLOR_INDEX_BLINK
    { CS_INVERSE,    CS_INVERSE,    ANSI_INVERSE,  sizeof(ANSI_INVERSE)-1,  T(COLOR_INVERSE),    3, T("%xi"), 3}, // COLOR_INDEX_INVERSE
    { CS_FG_BLACK,   CS_FOREGROUND, ANSI_BLACK,    sizeof(ANSI_BLACK)-1,    T(COLOR_FG_BLACK),   3, T("%xx"), 3}, // COLOR_INDEX_FG
    { CS_FG_RED,     CS_FOREGROUND, ANSI_RED,      sizeof(ANSI_RED)-1,      T(COLOR_FG_RED),     3, T("%xr"), 3},
    { CS_FG_GREEN,   CS_FOREGROUND, ANSI_GREEN,    sizeof(ANSI_GREEN)-1,    T(COLOR_FG_GREEN),   3, T("%xg"), 3},
    { CS_FG_YELLOW,  CS_FOREGROUND, ANSI_YELLOW,   sizeof(ANSI_YELLOW)-1,   T(COLOR_FG_YELLOW),  3, T("%xy"), 3},
    { CS_FG_BLUE,    CS_FOREGROUND, ANSI_BLUE,     sizeof(ANSI_BLUE)-1,     T(COLOR_FG_BLUE),    3, T("%xb"), 3},
    { CS_FG_MAGENTA, CS_FOREGROUND, ANSI_MAGENTA,  sizeof(ANSI_MAGENTA)-1,  T(COLOR_FG_MAGENTA), 3, T("%xm"), 3},
    { CS_FG_CYAN,    CS_FOREGROUND, ANSI_CYAN,     sizeof(ANSI_CYAN)-1,     T(COLOR_FG_CYAN),    3, T("%xc"), 3},
    { CS_FG_WHITE,   CS_FOREGROUND, ANSI_WHITE,    sizeof(ANSI_WHITE)-1,    T(COLOR_FG_WHITE),   3, T("%xw"), 3}, // COLOR_INDEX_FG_WHITE
    { CS_BG_BLACK,   CS_BACKGROUND, ANSI_BBLACK,   sizeof(ANSI_BBLACK)-1,   T(COLOR_BG_BLACK),   3, T("%xX"), 3}, // COLOR_INDEX_BG
    { CS_BG_RED,     CS_BACKGROUND, ANSI_BRED,     sizeof(ANSI_BRED)-1,     T(COLOR_BG_RED),     3, T("%xR"), 3},
    { CS_BG_GREEN,   CS_BACKGROUND, ANSI_BGREEN,   sizeof(ANSI_BGREEN)-1,   T(COLOR_BG_GREEN),   3, T("%xG"), 3},
    { CS_BG_YELLOW,  CS_BACKGROUND, ANSI_BYELLOW,  sizeof(ANSI_BYELLOW)-1,  T(COLOR_BG_YELLOW),  3, T("%xY"), 3},
    { CS_BG_BLUE,    CS_BACKGROUND, ANSI_BBLUE,    sizeof(ANSI_BBLUE)-1,    T(COLOR_BG_BLUE),    3, T("%xB"), 3},
    { CS_BG_MAGENTA, CS_BACKGROUND, ANSI_BMAGENTA, sizeof(ANSI_BMAGENTA)-1, T(COLOR_BG_MAGENTA), 3, T("%xM"), 3},
    { CS_BG_CYAN,    CS_BACKGROUND, ANSI_BCYAN,    sizeof(ANSI_BCYAN)-1,    T(COLOR_BG_CYAN),    3, T("%xC"), 3},
    { CS_BG_WHITE,   CS_BACKGROUND, ANSI_BWHITE,   sizeof(ANSI_BWHITE)-1,   T(COLOR_BG_WHITE),   3, T("%xW"), 3}  // COLOR_LAST_CODE
};

/*! \brief Validate ColorState.
 *
 * Checks (with assertions) that the given ColorState is valid.  This is
 * useful during development and debugging, but if any of the assertions are
 * false, the process ends.
 *
 * \param cs       ColorState.
 * \return         None.
 */

inline void ValidateColorState(ColorState cs)
{
    const ColorState Mask = static_cast<ColorState>(~(CS_FOREGROUND|CS_BACKGROUND|CS_ATTRS));
    mux_assert((Mask & cs) == 0);
    mux_assert((CS_FOREGROUND & cs) <= CS_FG_DEFAULT);
    mux_assert((CS_BACKGROUND & cs) <= CS_BG_DEFAULT);
}

inline ColorState UpdateColorState(ColorState cs, int iColorCode)
{
    return (cs & ~aColors[iColorCode].csMask) | aColors[iColorCode].cs;
}

// Maximum binary transition length is:
//
//   COLOR_RESET      "\xEF\x94\x80"
// + COLOR_INTENSE    "\xEF\x94\x81"
// + COLOR_UNDERLINE  "\xEF\x94\x84"
// + COLOR_BLINK      "\xEF\x94\x85"
// + COLOR_INVERSE    "\xEF\x94\x87"
// + COLOR_FG_RED     "\xEF\x98\x81"
// + COLOR_BG_WHITE   "\xEF\x9C\x87"
//
// Each of the seven codes is 3 bytes or 21 bytes total.
//
#define COLOR_MAXIMUM_BINARY_TRANSITION_LENGTH 21

// Generate the minimal color sequence that will transition from one color state
// to another.
//
static UTF8 *ColorTransitionBinary
(
    ColorState csCurrent,
    ColorState csNext,
    size_t *nTransition
)
{
    ValidateColorState(csCurrent);
    ValidateColorState(csNext);

    static UTF8 Buffer[COLOR_MAXIMUM_BINARY_TRANSITION_LENGTH+1];

    if (csCurrent == csNext)
    {
        *nTransition = 0;
        Buffer[0] = '\0';
        return Buffer;
    }
    size_t i = 0;

    // Do we need to go through the normal state?
    //
    if (  ((csCurrent & ~csNext) & CS_ATTRS)
       || (  (csNext & CS_BACKGROUND) == CS_BG_DEFAULT
          && (csCurrent & CS_BACKGROUND) != CS_BG_DEFAULT)
       || (  (csNext & CS_FOREGROUND) == CS_FG_DEFAULT
          && (csCurrent & CS_FOREGROUND) != CS_FG_DEFAULT))
    {
        memcpy(Buffer + i, COLOR_RESET, sizeof(COLOR_RESET)-1);
        i += sizeof(COLOR_RESET)-1;
        csCurrent = CS_NORMAL;
    }

    UINT16 tmp = csCurrent ^ csNext;
    if (CS_ATTRS & tmp)
    {
        for (unsigned int iAttr = COLOR_INDEX_ATTR; iAttr < COLOR_INDEX_FG; iAttr++)
        {
            if (aColors[iAttr].cs == (aColors[iAttr].csMask & tmp))
            {
                memcpy(Buffer + i, aColors[iAttr].pUTF, aColors[iAttr].nUTF);
                i += aColors[iAttr].nUTF;
            }
        }
    }

    if (CS_FOREGROUND & tmp)
    {
        unsigned int iForeground = COLOR_INDEX_FG + (CS_FOREGROUND & csNext);
        if (iForeground < COLOR_INDEX_FG + COLOR_INDEX_DEFAULT)
        {
            memcpy(Buffer + i, aColors[iForeground].pUTF, aColors[iForeground].nUTF);
            i += aColors[iForeground].nUTF;
        }
    }

    if (CS_BACKGROUND & tmp)
    {
        unsigned int iBackground = COLOR_INDEX_BG + ((CS_BACKGROUND & csNext) >> 4);
        if (iBackground < COLOR_INDEX_BG + COLOR_INDEX_DEFAULT)
        {
            memcpy(Buffer + i, aColors[iBackground].pUTF, aColors[iBackground].nUTF);
            i += aColors[iBackground].nUTF;
        }
    }
    Buffer[i] = '\0';
    *nTransition = i;
    return Buffer;
}

// Maximum binary transition to normal is:
//
//   COLOR_RESET      "\xEF\x94\x80"
//
// The code is 3 bytes.
//
#define COLOR_MAXIMUM_BINARY_NORMAL 3

// Generate the minimal color sequence that will transition from one color state
// to the normal state.
//
static const UTF8 *ColorBinaryNormal
(
    ColorState csCurrent,
    size_t *nTransition
)
{
    ValidateColorState(csCurrent);

    if (csCurrent == CS_NORMAL)
    {
        *nTransition = 0;
        return T("");
    }
    else
    {
        *nTransition = sizeof(COLOR_RESET) - 1;
        return T(COLOR_RESET);
    }
}

// Maximum escape transition length is:
//
//   COLOR_RESET      "%xn"
// + COLOR_INTENSE    "%xh"
// + COLOR_UNDERLINE  "%xu"
// + COLOR_BLINK      "%xf"
// + COLOR_INVERSE    "%xi"
// + COLOR_FG_RED     "%xr"
// + COLOR_BG_WHITE   "%xW"
//
// Each of the seven codes is 3 bytes or 21 bytes total.
//
#define COLOR_MAXIMUM_ESCAPE_TRANSITION_LENGTH 21

// Generate the minimal color %-sequence that will transition from one color state
// to another.
//
static UTF8 *ColorTransitionEscape
(
    ColorState csCurrent,
    ColorState csNext,
    size_t *nTransition
)
{
    ValidateColorState(csCurrent);
    ValidateColorState(csNext);

    static UTF8 Buffer[COLOR_MAXIMUM_ESCAPE_TRANSITION_LENGTH+1];

    if (csCurrent == csNext)
    {
        *nTransition = 0;
        Buffer[0] = '\0';
        return Buffer;
    }
    size_t i = 0;

    // Do we need to go through the normal state?
    //
    if (  ((csCurrent & ~csNext) & CS_ATTRS)
       || (  (csNext & CS_BACKGROUND) == CS_BG_DEFAULT
          && (csCurrent & CS_BACKGROUND) != CS_BG_DEFAULT)
       || (  (csNext & CS_FOREGROUND) == CS_FG_DEFAULT
          && (csCurrent & CS_FOREGROUND) != CS_FG_DEFAULT))
    {
        memcpy(Buffer + i, aColors[COLOR_INDEX_RESET].pEscape, aColors[COLOR_INDEX_RESET].nEscape);
        i += aColors[COLOR_INDEX_RESET].nEscape;
        csCurrent = CS_NORMAL;
    }

    UINT16 tmp = csCurrent ^ csNext;
    if (CS_ATTRS & tmp)
    {
        for (unsigned int iAttr = COLOR_INDEX_ATTR; iAttr < COLOR_INDEX_FG; iAttr++)
        {
            if (aColors[iAttr].cs == (aColors[iAttr].csMask & tmp))
            {
                memcpy(Buffer + i, aColors[iAttr].pEscape, aColors[iAttr].nEscape);
                i += aColors[iAttr].nEscape;
            }
        }
    }

    if (CS_FOREGROUND & tmp)
    {
        unsigned int iForeground = COLOR_INDEX_FG + (CS_FOREGROUND & csNext);
        if (iForeground < COLOR_INDEX_FG + COLOR_INDEX_DEFAULT)
        {
            memcpy(Buffer + i, aColors[iForeground].pEscape, aColors[iForeground].nEscape);
            i += aColors[iForeground].nEscape;
        }
    }

    if (CS_BACKGROUND & tmp)
    {
        unsigned int iBackground = COLOR_INDEX_BG + ((CS_BACKGROUND & csNext) >> 4);
        if (iBackground < COLOR_INDEX_BG + COLOR_INDEX_DEFAULT)
        {
            memcpy(Buffer + i, aColors[iBackground].pEscape, aColors[iBackground].nEscape);
            i += aColors[iBackground].nEscape;
        }
    }
    Buffer[i] = '\0';
    *nTransition = i;
    return Buffer;
}

// Maximum ANSI transition length is:
//
//   ANSI_NORMAL    "\033[0m"
// + ANSI_HILITE    "\033[1m"
// + ANSI_UNDER     "\033[4m"
// + ANSI_BLINK     "\033[5m"
// + ANSI_INVERSE   "\033[7m"
// + ANSI_RED       "\033[31m"
// + ANSI_BWHITE    "\033[47m"
//
// Five of the seven codes are 4 bytes, and two are 5 bytes, or 30 bytes total.
//
#define COLOR_MAXIMUM_ANSI_TRANSITION_LENGTH 30

// Generate the minimal ANSI sequence that will transition from one color state
// to another.
//
static UTF8 *ColorTransitionANSI
(
    ColorState csCurrent,
    ColorState csNext,
    size_t *nTransition,
    bool bNoBleed = false
)
{
    ValidateColorState(csCurrent);
    ValidateColorState(csNext);

    static UTF8 Buffer[COLOR_MAXIMUM_ANSI_TRANSITION_LENGTH+1];

    if (bNoBleed)
    {
        if ((csCurrent & CS_FOREGROUND) == CS_FG_DEFAULT)
        {
            csCurrent = (csCurrent & ~CS_FOREGROUND) | CS_FG_WHITE;
        }

        if ((csNext & CS_FOREGROUND) == CS_FG_DEFAULT)
        {
            csNext = (csNext & ~CS_FOREGROUND) | CS_FG_WHITE;
        }
    }

    if (csCurrent == csNext)
    {
        *nTransition = 0;
        Buffer[0] = '\0';
        return Buffer;
    }
    size_t i = 0;

    // Do we need to go through the normal state?
    //
    if (  ((csCurrent & ~csNext) & CS_ATTRS)
       || (  (csNext & CS_BACKGROUND) == CS_BG_DEFAULT
          && (csCurrent & CS_BACKGROUND) != CS_BG_DEFAULT)
       || (  (csNext & CS_FOREGROUND) == CS_FG_DEFAULT
          && (csCurrent & CS_FOREGROUND) != CS_FG_DEFAULT))
    {
        memcpy(Buffer + i, ANSI_NORMAL, sizeof(ANSI_NORMAL)-1);
        i += sizeof(ANSI_NORMAL)-1;
        csCurrent = CS_NORMAL;
    }

    UINT16 tmp = csCurrent ^ csNext;
    if (CS_ATTRS & tmp)
    {
        for (unsigned int iAttr = COLOR_INDEX_ATTR; iAttr < COLOR_INDEX_FG; iAttr++)
        {
            if (aColors[iAttr].cs == (aColors[iAttr].csMask & tmp))
            {
                memcpy(Buffer + i, aColors[iAttr].pAnsi, aColors[iAttr].nAnsi);
                i += aColors[iAttr].nAnsi;
            }
        }
    }

    if (CS_FOREGROUND & tmp)
    {
        unsigned int iForeground = COLOR_INDEX_FG + (CS_FOREGROUND & csNext);
        if (iForeground < COLOR_INDEX_FG + COLOR_INDEX_DEFAULT)
        {
            memcpy(Buffer + i, aColors[iForeground].pAnsi, aColors[iForeground].nAnsi);
            i += aColors[iForeground].nAnsi;
        }
    }

    if (CS_BACKGROUND & tmp)
    {
        unsigned int iBackground = COLOR_INDEX_BG + ((CS_BACKGROUND & csNext) >> 4);
        if (iBackground < COLOR_INDEX_BG + COLOR_INDEX_DEFAULT)
        {
            memcpy(Buffer + i, aColors[iBackground].pAnsi, aColors[iBackground].nAnsi);
            i += aColors[iBackground].nAnsi;
        }
    }
    Buffer[i] = '\0';
    *nTransition = i;
    return Buffer;
}

UTF8 *convert_color(const UTF8 *pString, bool bNoBleed)
{
    static UTF8 aBuffer[2*LBUF_SIZE];
    UTF8 *pBuffer = aBuffer;
    while ('\0' != *pString)
    {
        unsigned int iCode = mux_color(pString);
        if (COLOR_NOTCOLOR == iCode)
        {
            utf8_safe_chr(pString, aBuffer, &pBuffer);
        }
        else
        {
            memcpy(pBuffer, aColors[iCode].pAnsi, aColors[iCode].nAnsi);
            pBuffer += aColors[iCode].nAnsi;
        }
        pString = utf8_NextCodePoint(pString);

        if (  bNoBleed
           && COLOR_INDEX_RESET == iCode
           && COLOR_INDEX_FG_WHITE != mux_color(pString))
        {
            memcpy(pBuffer, aColors[COLOR_INDEX_FG_WHITE].pAnsi,
                            aColors[COLOR_INDEX_FG_WHITE].nAnsi);
            pBuffer += aColors[COLOR_INDEX_FG_WHITE].nAnsi;
        }
    }
    *pBuffer = '\0';
    return aBuffer;
}

UTF8 *strip_color(const UTF8 *pString, size_t *pnBytes, size_t *pnPoints)
{
    static UTF8 aBuffer[LBUF_SIZE];
    UTF8 *pBuffer = aBuffer;

    if (NULL == pString)
    {
        if (NULL != pnBytes)
        {
            *pnBytes = 0;
        }
        if (NULL != pnPoints)
        {
            *pnPoints = 0;
        }
        *pBuffer = '\0';
        return aBuffer;
    }

    size_t nPoints = 0;
    while ('\0' != *pString)
    {
        if (COLOR_NOTCOLOR == mux_color(pString))
        {
            utf8_safe_chr(pString, aBuffer, &pBuffer);
            nPoints++;
        }
        pString = utf8_NextCodePoint(pString);
    }
    *pBuffer = '\0';
    if (NULL != pnBytes)
    {
        *pnBytes = pBuffer - aBuffer;
    }
    if (NULL != pnPoints)
    {
        *pnPoints = nPoints;
    }
    return aBuffer;
}

typedef struct
{
    int len;
    const UTF8 *p;
} LITERAL_STRING_STRUCT;

#define NUM_MU_SUBS 14
static LITERAL_STRING_STRUCT MU_Substitutes[NUM_MU_SUBS] =
{
    { 1, T(" ")  },  // 0
    { 1, T(" ")  },  // 1
    { 2, T("%t") },  // 2
    { 2, T("%r") },  // 3
    { 0,         NULL },  // 4
    { 2, T("%b") },  // 5
    { 2, T("%%") },  // 6
    { 2, T("%(") },  // 7
    { 2, T("%)") },  // 8
    { 2, T("%[") },  // 9
    { 2, T("%]") },  // 10
    { 2, T("%{") },  // 11
    { 2, T("%}") },  // 12
    { 2, T("\\\\") } // 13
};

const unsigned char MU_EscapeConvert[256] =
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
//
    0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 4, 0, 0,  // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1
    1, 0, 0, 0, 0, 6, 0, 0, 7, 8, 0, 0, 0, 0, 0, 0,  // 2
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 3
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 4
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,13,10, 0, 0,  // 5
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 6
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,11, 0,12, 0, 0,  // 7

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // E
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   // F
};

const unsigned char MU_EscapeNoConvert[256] =
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
//
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 4, 0, 0,  // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 2
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 3
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 4
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 5
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 6
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 7

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // E
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   // F
};

// Convert raw character sequences into MUX substitutions (type = 1)
// or strips them (type = 0).
//
UTF8 *translate_string(const UTF8 *pString, bool bConvert)
{
    static UTF8 szTranslatedString[LBUF_SIZE];
    UTF8 *pTranslatedString = szTranslatedString;

    if (!pString)
    {
        *pTranslatedString = '\0';
        return szTranslatedString;
    }

    ColorState csCurrent = CS_NOBLEED;
    ColorState csPrevious = csCurrent;

    const UTF8 *MU_EscapeChar = (bConvert)? MU_EscapeConvert : MU_EscapeNoConvert;
    while ('\0' != *pString)
    {
        unsigned int iCode = mux_color(pString);
        if (COLOR_NOTCOLOR == iCode)
        {
            size_t nTransition = 0;
            if (bConvert)
            {
                UTF8 *pTransition = ColorTransitionEscape(csPrevious, csCurrent, &nTransition);
                safe_str(pTransition, szTranslatedString, &pTranslatedString);
                csPrevious = csCurrent;
            }

            UTF8 ch = pString[0];
            unsigned char code = MU_EscapeChar[ch];
            if (  0 < code
               && code < NUM_MU_SUBS)
            {
                if (  ' ' == ch
                   && ' ' == pString[1])
                {
                    code = 5;
                }
                safe_copy_buf(MU_Substitutes[code].p,
                    MU_Substitutes[code].len, szTranslatedString,
                    &pTranslatedString);
            }
            else
            {
                utf8_safe_chr(pString, szTranslatedString, &pTranslatedString);
            }
        }
        else
        {
            csCurrent = UpdateColorState(csCurrent, iCode);
        }
        pString = utf8_NextCodePoint(pString);
    }
    *pTranslatedString = '\0';
    return szTranslatedString;
}

/* ---------------------------------------------------------------------------
 * munge_space: Compress multiple spaces to one space, also remove leading and
 * trailing spaces.
 */
UTF8 *munge_space(const UTF8 *string)
{
    UTF8 *buffer = alloc_lbuf("munge_space");
    const UTF8 *p = string;
    UTF8 *q = buffer;

    if (p)
    {
        // Remove initial spaces.
        //
        while (mux_isspace(*p))
            p++;

        while (*p)
        {
            while (*p && !mux_isspace(*p))
                *q++ = *p++;

            while (mux_isspace(*p))
            {
                p++;
            }

            if (*p)
                *q++ = ' ';
        }
    }

    // Remove terminal spaces and terminate string.
    //
    *q = '\0';
    return buffer;
}

/* ---------------------------------------------------------------------------
 * trim_spaces: Remove leading and trailing spaces and space-compress internal
 * spaces.
 */
UTF8 *trim_spaces(const UTF8 *string)
{
    UTF8 *buffer = alloc_lbuf("trim_spaces");
    const UTF8 *p = string;
    UTF8 *q = buffer;

    if (p)
    {
        // Remove initial spaces.
        //
        while (mux_isspace(*p))
        {
            p++;
        }

        while (*p)
        {
            // Copy non-space characters.
            //
            while (*p && !mux_isspace(*p))
            {
                *q++ = *p++;
            }

            // Compress spaces.
            //
            while (mux_isspace(*p))
            {
                p++;
            }

            // Leave one space.
            //
            if (*p)
            {
                *q++ = ' ';
            }
        }
    }

    // Terminate string.
    //
    *q = '\0';
    return buffer;
}

/*
 * ---------------------------------------------------------------------------
 * * grabto: Return portion of a string up to the indicated character.  Also
 * * returns a modified pointer to the string ready for another call.
 */

UTF8 *grabto(UTF8 **str, UTF8 targ)
{
    UTF8 *savec, *cp;

    if (!str || !*str || !**str)
        return NULL;

    savec = cp = *str;
    while (*cp && *cp != targ)
        cp++;
    if (*cp)
        *cp++ = '\0';
    *str = cp;
    return savec;
}

int string_compare(const UTF8 *s1, const UTF8 *s2)
{
    if (  mudstate.bStandAlone
       || mudconf.space_compress)
    {
        while (mux_isspace(*s1))
        {
            s1++;
        }
        while (mux_isspace(*s2))
        {
            s2++;
        }

        while (  *s1 && *s2
              && (  (mux_tolower_ascii(*s1) == mux_tolower_ascii(*s2))
                 || (mux_isspace(*s1) && mux_isspace(*s2))))
        {
            if (mux_isspace(*s1) && mux_isspace(*s2))
            {
                // skip all other spaces.
                //
                do
                {
                    s1++;
                } while (mux_isspace(*s1));

                do
                {
                    s2++;
                } while (mux_isspace(*s2));
            }
            else
            {
                s1++;
                s2++;
            }
        }
        if (  *s1
           && *s2)
        {
            return 1;
        }

        if (mux_isspace(*s1))
        {
            while (mux_isspace(*s1))
            {
                s1++;
            }
            return *s1;
        }
        if (mux_isspace(*s2))
        {
            while (mux_isspace(*s2))
            {
                s2++;
            }
            return *s2;
        }
        if (  *s1
           || *s2)
        {
            return 1;
        }
        return 0;
    }
    else
    {
        return mux_stricmp(s1, s2);
    }
}

int string_prefix(const UTF8 *string, const UTF8 *prefix)
{
    int count = 0;

    while (*string && *prefix
          && (mux_tolower_ascii(*string) == mux_tolower_ascii(*prefix)))
    {
        string++, prefix++, count++;
    }
    if (*prefix == '\0')
    {
        // Matched all of prefix.
        //
        return count;
    }
    else
    {
        return 0;
    }
}

/*
 * accepts only nonempty matches starting at the beginning of a word
 */

const UTF8 *string_match(const UTF8 *src, const UTF8 *sub)
{
    if ((*sub != '\0') && (src))
    {
        while (*src)
        {
            if (string_prefix(src, sub))
            {
                return src;
            }

            // else scan to beginning of next word
            //
            while (mux_isalnum(*src))
            {
                src++;
            }
            while (*src && !mux_isalnum(*src))
            {
                src++;
            }
        }
    }
    return 0;
}

/*
 * ---------------------------------------------------------------------------
 * * replace_string: Returns an lbuf containing string STRING with all occurances
 * * of OLD replaced by NEW. OLD and NEW may be different lengths.
 * * (mitch 1 feb 91)
 */

UTF8 *replace_string(const UTF8 *old, const UTF8 *new0, const UTF8 *s)
{
    if (!s)
    {
        return NULL;
    }
    size_t olen = strlen((char *)old);
    UTF8 *result = alloc_lbuf("replace_string");
    UTF8 *r = result;
    while (*s)
    {
        // Find next occurrence of the first character of OLD string.
        //
        const UTF8 *p = (UTF8 *)strchr((char *)s, old[0]);
        if (  olen
           && p)
        {
            // Copy up to the next occurrence of the first char of OLD.
            //
            size_t n = p - s;
            if (n)
            {
                safe_copy_buf(s, n, result, &r);
                s += n;
            }

            // If we are really at an complete OLD, append NEW to the result
            // and bump the input string past the occurrence of OLD.
            // Otherwise, copy the character and try matching again.
            //
            if (!strncmp((char *)old, (char *)s, olen))
            {
                safe_str(new0, result, &r);
                s += olen;
            }
            else
            {
                safe_chr(*s, result, &r);
                s++;
            }
        }
        else
        {
            // Finish copying source string. No matches. No further
            // work to perform.
            //
            safe_str(s, result, &r);
            break;
        }
    }
    *r = '\0';
    return result;
}

// ---------------------------------------------------------------------------
// replace_tokens: Performs ## and #@ substitution.
//
UTF8 *replace_tokens
(
    const UTF8 *s,
    const UTF8 *pBound,
    const UTF8 *pListPlace,
    const UTF8 *pSwitch
)
{
    if (!s)
    {
        return NULL;
    }
    UTF8 *result = alloc_lbuf("replace_tokens");
    UTF8 *r = result;

    while (*s)
    {
        // Find next '#'.
        //
        const UTF8 *p = (UTF8 *)strchr((char *)s, '#');
        if (p)
        {
            // Copy up to the next occurrence of the first character.
            //
            size_t n = p - s;
            if (n)
            {
                safe_copy_buf(s, n, result, &r);
                s += n;
            }

            if (  s[1] == '#'
               && pBound)
            {
                // BOUND_VAR
                //
                safe_str(pBound, result, &r);
                s += 2;
            }
            else if (  s[1] == '@'
                    && pListPlace)
            {
                // LISTPLACE_VAR
                //
                safe_str(pListPlace, result, &r);
                s += 2;
            }
            else if (  s[1] == '$'
                    && pSwitch)
            {
                // SWITCH_VAR
                //
                safe_str(pSwitch, result, &r);
                s += 2;
            }
            else
            {
                safe_chr(*s, result, &r);
                s++;
            }
        }
        else
        {
            // Finish copying source string. No matches. No further
            // work to perform.
            //
            safe_str(s, result, &r);
            break;
        }
    }
    *r = '\0';
    return result;
}

bool minmatch(const UTF8 *str, const UTF8 *target, int min)
{
    while (*str && *target
          && (mux_tolower_ascii(*str) == mux_tolower_ascii(*target)))
    {
        str++;
        target++;
        min--;
    }
    if (*str)
    {
        return false;
    }
    if (!*target)
    {
        return true;
    }
    return (min <= 0);
}

// --------------------------------------------------------------------------
// StringCloneLen: allocate memory and copy string
//
UTF8 *StringCloneLen(const UTF8 *str, size_t nStr)
{
    UTF8 *buff = (UTF8 *)MEMALLOC(nStr+1);
    if (buff)
    {
        memcpy(buff, str, nStr);
        buff[nStr] = '\0';
    }
    else
    {
        ISOUTOFMEMORY(buff);
    }
    return buff;
}

// --------------------------------------------------------------------------
// StringClone: allocate memory and copy string
//
UTF8 *StringClone(const UTF8 *str)
{
    return StringCloneLen(str, strlen((char *)str));
}

#if 0
// --------------------------------------------------------------------------
// BufferCloneLen: allocate memory and copy buffer
//
UTF8 *BufferCloneLen(const UTF8 *pBuffer, unsigned int nBuffer)
{
    UTF8 *buff = (UTF8 *)MEMALLOC(nBuffer);
    ISOUTOFMEMORY(buff);
    memcpy(buff, pBuffer, nBuffer);
    return buff;
}
#endif // 0

/* ---------------------------------------------------------------------------
 * safe_copy_str - Copy buffers, watching for overflows.
 */

void safe_copy_str(const UTF8 *src, UTF8 *buff, UTF8 **bufp, size_t nSizeOfBuffer)
{
    if (src == NULL) return;

    UTF8 *tp = *bufp;
    UTF8 *maxtp = buff + nSizeOfBuffer;
    while (tp < maxtp && *src)
    {
        *tp++ = *src++;
    }
    *bufp = buff + TrimPartialSequence(tp - buff, buff);
}

void safe_copy_str_lbuf(const UTF8 *src, UTF8 *buff, UTF8 **bufp)
{
    if (src == NULL)
    {
        return;
    }

    UTF8 *tp = *bufp;
    UTF8 *maxtp = buff + LBUF_SIZE - 1;
    while (tp < maxtp && *src)
    {
        *tp++ = *src++;
    }
    *bufp = buff + TrimPartialSequence(tp - buff, buff);
}

size_t safe_copy_buf(__in_ecount(nLen) const UTF8 *src, size_t nLen, __in UTF8 *buff, __deref_inout UTF8 **bufc)
{
    size_t left = LBUF_SIZE - (*bufc - buff) - 1;
    if (left < nLen)
    {
        nLen = TrimPartialSequence(left, src);
    }
    memcpy(*bufc, src, nLen);
    *bufc += nLen;
    return nLen;
}

size_t safe_fill(UTF8 *buff, UTF8 **bufc, UTF8 chFill, size_t nSpaces)
{
    // Check for buffer limits.
    //
    size_t nBufferAvailable = LBUF_SIZE - (*bufc - buff) - 1;
    if (nSpaces > nBufferAvailable)
    {
        nSpaces = nBufferAvailable;
    }

    // Fill with spaces.
    //
    memset(*bufc, chFill, nSpaces);
    *bufc += nSpaces;
    return nSpaces;
}

void utf8_safe_chr(const UTF8 *src, UTF8 *buff, UTF8 **bufc)
{
    size_t nLen;
    size_t nLeft;
    if (  NULL == src
       || UTF8_CONTINUE <= (nLen = utf8_FirstByte[*src])
       || (nLeft = LBUF_SIZE - (*bufc - buff) - 1) < nLen)
    {
        return;
    }
    memcpy(*bufc, src, nLen);
    *bufc += nLen;
}

UTF8 *ConvertToUTF8
(
    UTF32  ch
)
{
    static UTF8 buffer[6];
    const UTF32 byteMask = 0xBF;
    const UTF32 byteMark = 0x80;

    if (ch < (UTF32)0x80)
    {
        // ASCII
        //
        buffer[1] = '\0';
        buffer[0] = static_cast<UTF8>(ch);
    }
    else if (ch < (UTF32)0x800)
    {
        buffer[2] = '\0';
        buffer[1] = static_cast<char>((ch | byteMark) & byteMask);
        ch >>= 6;
        buffer[0] = static_cast<char>(0xC0 | ch);
    }
    else if (ch < (UTF32)0x10000)
    {
        if (  UNI_SUR_HIGH_START <= ch
           && ch <= UNI_SUR_LOW_END)
        {
            buffer[0] = '\0';
            return buffer;
        }
        buffer[3] = '\0';
        buffer[2] =static_cast<char>((ch | byteMark) & byteMask);
        ch >>= 6;
        buffer[1] = static_cast<char>((ch | byteMark) & byteMask);
        ch >>= 6;
        buffer[0] = static_cast<char>(0xE0 | ch);
    }
    else if (ch <= UNI_MAX_LEGAL_UTF32)
    {
        buffer[4] = '\0';
        buffer[3] = static_cast<char>((ch | byteMark) & byteMask);
        ch >>= 6;
        buffer[2] = static_cast<char>((ch | byteMark) & byteMask);
        ch >>= 6;
        buffer[1] = static_cast<char>((ch | byteMark) & byteMask);
        ch >>= 6;
        buffer[0] = static_cast<char>(0xF0 | ch);
    }
    return buffer;
}

UTF16 *ConvertToUTF16(UTF32 ch)
{
    static UTF16 buffer[3];
    if (  ch < UNI_SUR_HIGH_START
       || UNI_SUR_LOW_END < ch)
    {
        // This is the common case.
        //
        buffer[0] = (UTF16)ch;
        buffer[1] = 0x0000;
        return buffer;
    }
    else if (ch <= UNI_MAX_LEGAL_UTF32)
    {
        const int halfShift  = 10;
        const UTF32 halfBase = 0x0010000UL;
        const UTF32 halfMask = 0x3FFUL;

        ch -= halfBase;
        buffer[0] = (UTF16)((ch >> halfShift) + UNI_SUR_HIGH_START);
        buffer[1] = (UTF16)((ch & halfMask) + UNI_SUR_LOW_START);
        buffer[2] = 0x0000;
    }
    else
    {
        buffer[0] = UNI_REPLACEMENT_CHAR;
        buffer[1] = 0x0000;
    }
    return buffer;
}

UTF32 ConvertFromUTF8(const UTF8 *pString)
{
    size_t t = utf8_FirstByte[*pString];
    if (UTF8_CONTINUE <= t)
    {
        return UNI_EOF;
    }

    UTF32 ch;
    if (1 == t)
    {
        // This is the most common case, and the value is always smaller than
        // UNI_SUR_HIGH_START.
        //
        return pString[0];
    }
    else if (2 == t)
    {
        if (UTF8_CONTINUE != utf8_FirstByte[pString[1]])
        {
            return UNI_EOF;
        }
        ch =  ((UTF32)(pString[0] & 0x1F) <<  6)
           |  ((UTF32)(pString[1] & 0x3F)      );
    }
    else if (3 == t)
    {
        if (  UTF8_CONTINUE != utf8_FirstByte[pString[1]]
           || UTF8_CONTINUE != utf8_FirstByte[pString[2]])
        {
            return UNI_EOF;
        }
        ch = ((UTF32)(pString[0] & 0x1F) << 12)
           | ((UTF32)(pString[1] & 0x3F) <<  6)
           | ((UTF32)(pString[2] & 0x3F)      );
    }
    else if (4 == t)
    {
        if (  UTF8_CONTINUE != utf8_FirstByte[pString[1]]
           || UTF8_CONTINUE != utf8_FirstByte[pString[2]]
           || UTF8_CONTINUE != utf8_FirstByte[pString[3]])
        {
            return UNI_EOF;
        }
        ch = ((UTF32)(pString[0] & 0x1F) << 18)
           | ((UTF32)(pString[1] & 0x3F) << 12)
           | ((UTF32)(pString[2] & 0x3F) <<  6)
           | ((UTF32)(pString[3] & 0x3F)      );
    }
    else
    {
        return UNI_EOF;
    }

    if (  ch < UNI_SUR_HIGH_START
       || (  UNI_SUR_LOW_END < ch
          && ch <= UNI_MAX_LEGAL_UTF32))
    {
        return ch;
    }
    else
    {
        return UNI_REPLACEMENT_CHAR;
    }
}

size_t ConvertFromUTF16(UTF16 *pString, UTF32 &ch)
{
    ch = pString[0];
    if (  ch < UNI_SUR_HIGH_START
       || UNI_SUR_LOW_END < ch)
    {
        // This is the most-common case.
        //
        return 1;
    }
    else if (ch <= UNI_SUR_HIGH_END)
    {
        UTF32 ch2 = pString[1];
        if (  UNI_SUR_LOW_START <= ch2
           && ch2 <= UNI_SUR_LOW_END)
        {
            const int halfShift  = 10;
            const UTF32 halfBase = 0x0010000UL;

            ch = ((ch - UNI_SUR_HIGH_START) << halfShift)
               + (ch2 - UNI_SUR_LOW_START)
               + halfBase;
            return 2;
        }
    }
    ch = UNI_EOF;
    return 0;
}

UTF16 *ConvertFromUTF8ToUTF16(const UTF8 *pString, size_t *pnString)
{
    static UTF16 buffer[2*LBUF_SIZE];
    UTF16 *p = buffer;

    *pnString = 0;
    while ('\0' != *pString)
    {
        UTF32 ch = ConvertFromUTF8(pString);
        if (UNI_EOF == ch)
        {
            return NULL;
        }

        UTF16 *q = ConvertToUTF16(ch);
        while (0x0000 != *q)
        {
            *p++ = *q++;
        }
        pString = utf8_NextCodePoint(pString);
    }
    *p = '\0';
    *pnString = p - buffer;
    return buffer;
}

// We want to remove mal-formed ESC sequences completely and convert the
// well-formed ones.
//
UTF8 *ConvertToUTF8(const char *p, size_t *pn)
{
    *pn = 0;

    static UTF8 aBuffer[LBUF_SIZE];
    UTF8 *pBuffer = aBuffer;

    while ('\0' != *p)
    {
        if (ESC_CHAR != *p)
        {
            const UTF8 *q = latin1_utf8[(unsigned char)*p];
            utf8_safe_chr(q, aBuffer, &pBuffer);
            p++;
        }
        else
        {
            // We have an ANSI sequence.
            //
            p++;
            if ('[' == *p)
            {
                p++;
                const char *q = p;
                while (ANSI_TokenTerminatorTable[(unsigned char)*q] == 0)
                {
                    q++;
                }

                if ('\0' != q[0])
                {
                    // The segment [p,q) should contain a list of semi-color delimited codes.
                    //
                    const char *r = p;
                    while (r != q)
                    {
                        while (  r != q
                              && ';' != r[0])
                        {
                            r++;
                        }

                        // The segment [p,r) should contain one code.
                        //
                        size_t n = r - p;
                        const UTF8 *s = NULL;
                        switch (n)
                        {
                        case 1:
                            if ('0' == *p)
                            {
                                s = aColors[COLOR_INDEX_RESET].pUTF;
                            }
                            else if ('1' == *p)
                            {
                                s = aColors[COLOR_INDEX_INTENSE].pUTF;
                            }
                            else if ('4' == *p)
                            {
                                s = aColors[COLOR_INDEX_UNDERLINE].pUTF;
                            }
                            else if ('5' == *p)
                            {
                                s = aColors[COLOR_INDEX_BLINK].pUTF;
                            }
                            else if ('7' == *p)
                            {
                                s = aColors[COLOR_INDEX_INVERSE].pUTF;
                            }
                            break;

                        case 2:
                            if ('3' == *p)
                            {
                                unsigned int iCode = COLOR_INDEX_FG + (p[1] - '0');
                                if (  COLOR_INDEX_FG <= iCode
                                   && iCode < COLOR_INDEX_BG)
                                {
                                    s = aColors[iCode].pUTF;
                                }
                            }
                            else if ('4' == *p)
                            {
                                unsigned int iCode = COLOR_INDEX_BG + (p[1] - '0');
                                if (  COLOR_INDEX_BG <= iCode
                                   && iCode <= COLOR_LAST_CODE)
                                {
                                    s = aColors[iCode].pUTF;
                                }
                            }
                            break;
                        }

                        if (NULL != s)
                        {
                            utf8_safe_chr(s, aBuffer, &pBuffer);
                        }

                        while (  r != q
                              && ';' == r[0])
                        {
                            r++;
                        }
                        p = r;
                    }

                    // Eat trailing terminator.
                    //
                    p = q + 1;
                }
                else
                {
                    // Skip to end of mal-formed ANSI sequence.
                    //
                    p = q;
                }
            }
        }
    }
    *pBuffer = '\0';
    *pn = pBuffer - aBuffer;
    return aBuffer;
}

// mux_strncpy: Copies up to specified number of chars from source.
// Note: unlike strncpy(), this null-terminates after copying.
//
void mux_strncpy(UTF8 *dest, const UTF8 *src, size_t nSizeOfBuffer)
{
    if (  NULL == src
       || 0 == nSizeOfBuffer)
    {
        return;
    }

    size_t i = 0;
    while (  i < nSizeOfBuffer
          && '\0' != src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

bool matches_exit_from_list(const UTF8 *str, const UTF8 *pattern)
{
    const UTF8 *s;

    while (*pattern)
    {
        for (s = str;   // check out this one
             ( *s
             && (mux_tolower_ascii(*s) == mux_tolower_ascii(*pattern))
             && *pattern
             && (*pattern != EXIT_DELIMITER));
             s++, pattern++) ;

        // Did we match it all?
        //
        if (*s == '\0')
        {
            // Make sure nothing afterwards
            //
            while (mux_isspace(*pattern))
            {
                pattern++;
            }

            // Did we get it?
            //
            if (  !*pattern
               || (*pattern == EXIT_DELIMITER))
            {
                return true;
            }
        }
        // We didn't get it, find next string to test
        //
        while (  *pattern
              && *pattern++ != EXIT_DELIMITER)
        {
            ; // Nothing.
        }
        while (mux_isspace(*pattern))
        {
            pattern++;
        }
    }
    return false;
}

// mux_strtok_src, mux_strtok_ctl, mux_strtok_parse.
//
// These three functions work together to replace the functionality of the
// strtok() C runtime library function. Call mux_strtok_src() first with
// the string to parse, then mux_strtok_ctl() with the control
// characters, and finally mux_strtok_parse() to parse out the tokens.
//
// You may call mux_strtok_ctl() to change the set of control characters
// between mux_strtok_parse() calls, however keep in mind that the parsing
// may not occur how you intend it to as mux_strtok_parse() does not
// consume -all- of the controlling delimiters that separate two tokens.
// It consumes only the first one.
//
void mux_strtok_src(MUX_STRTOK_STATE *tts, UTF8 *arg_pString)
{
    if (!tts || !arg_pString) return;

    // Remember the string to parse.
    //
    tts->pString = arg_pString;
}

void mux_strtok_ctl(MUX_STRTOK_STATE *tts, const UTF8 *pControl)
{
    if (!tts || !pControl) return;

    // No character is a control character.
    //
    memset(tts->aControl, 0, sizeof(tts->aControl));

    // The NULL character is always a control character.
    //
    tts->aControl[0] = 1;

    // Record the user-specified control characters.
    //
    while (*pControl)
    {
        tts->aControl[(unsigned char)*pControl] = 1;
        pControl++;
    }
}

UTF8 *mux_strtok_parseLEN(MUX_STRTOK_STATE *tts, size_t *pnLen)
{
    *pnLen = 0;
    if (!tts)
    {
        return NULL;
    }
    UTF8 *p = tts->pString;
    if (!p)
    {
        return NULL;
    }

    // Skip over leading control characters except for the NUL character.
    //
    while (tts->aControl[(unsigned char)*p] && *p)
    {
        p++;
    }

    UTF8 *pReturn = p;

    // Skip over non-control characters.
    //
    while (tts->aControl[(unsigned char)*p] == 0)
    {
        p++;
    }

    // What is the length of this token?
    //
    *pnLen = p - pReturn;

    // Terminate the token with a NUL.
    //
    if (p[0])
    {
        // We found a non-NUL delimiter, so the next call will begin parsing
        // on the character after this one.
        //
        tts->pString = p+1;
    }
    else
    {
        // We hit the end of the string, so the end of the string is where
        // the next call will begin.
        //
        tts->pString = p;
    }

    // Did we find a token?
    //
    if (*pnLen > 0)
    {
        return pReturn;
    }
    else
    {
        return NULL;
    }
}

UTF8 *mux_strtok_parse(MUX_STRTOK_STATE *tts)
{
    size_t nLen;
    UTF8 *p = mux_strtok_parseLEN(tts, &nLen);
    if (p)
    {
        p[nLen] = '\0';
    }
    return p;
}

mux_field StripTabsAndTruncate
(
    const UTF8 *pString,
    UTF8 *pBuffer,
    size_t nLength,
    size_t nWidth0
)
{
    mux_field  fldOutput(0, 0);

    if (  NULL == pBuffer
       || NULL == pString
       || 0 == nLength
       || 0 == nWidth0
       || '\0' == pString[0])
    {
        if (  NULL != pBuffer
           && 0 < nLength)
        {
            pBuffer[0] = '\0';
        }
        return fldOutput;
    }

    LBUF_OFFSET nWidth = static_cast<LBUF_OFFSET>(nWidth0);
    if (nLength < nWidth)
    {
        nWidth = static_cast<LBUF_OFFSET>(nLength);
    }

    mux_cursor curPos = CursorMin;
    mux_field  fldLimit(nLength, nWidth);

    mux_field  fldTransition(0, 0);
    mux_field  fldNormal(0, 0);
    const UTF8 *pTransition = NULL, *pNormal = NULL;
    size_t nNormalBytes = 0, nTransition = 0;
    ColorState csCurrent = CS_NORMAL, csNext = CS_NORMAL;

    while ('\0' != pString[curPos.m_byte])
    {
        int iCode = mux_color(pString + curPos.m_byte);
        mux_cursor curPoint(utf8_FirstByte[pString[curPos.m_byte]], 1);
        if (COLOR_NOTCOLOR != iCode)
        {
            csNext = UpdateColorState(csNext, iCode);
        }
        else if (NULL == strchr("\r\n\t", pString[curPos.m_byte]))
        {
            mux_field  fldPoint(utf8_FirstByte[pString[curPos.m_byte]], 1);
            if (csCurrent != csNext)
            {
                pTransition = ColorTransitionBinary(csCurrent, csNext, &nTransition);
                pNormal = ColorBinaryNormal(csNext, &nNormalBytes);
                fldNormal(nNormalBytes, 0);
            }
            else
            {
                nTransition = 0;
            }
            fldTransition(nTransition, 0);
            if (fldOutput + fldTransition + fldPoint + fldNormal <= fldLimit)
            {
                if (0 < nTransition)
                {
                    memcpy(pBuffer + fldOutput.m_byte, pTransition, nTransition);
                    csCurrent = csNext;
                }
                fldOutput += fldTransition;

                for (size_t j = 0; j < fldPoint.m_byte; j++)
                {
                    pBuffer[fldOutput.m_byte + j] = pString[curPos.m_byte + j];
                }
                fldOutput += fldPoint;
            }
            else
            {
                break;
            }
        }
        curPos += curPoint;
    }

    if (csCurrent != csNext)
    {
        pNormal = ColorBinaryNormal(csCurrent, &nNormalBytes);
        fldNormal(nNormalBytes, 0);
    }

    if (  0 < nNormalBytes
       && fldOutput + fldNormal <= fldLimit)
    {
        memcpy(pBuffer + fldOutput.m_byte, pNormal, nNormalBytes);
        fldOutput += fldNormal;
    }

    pBuffer[fldOutput.m_byte] = '\0';
    return fldOutput;
}

// TruncateToBuffer()
//
// pString is parsed into alternating runs of text and color.  These runs are
// then encoded into the given buffer with proper truncation.  Every run of
// text will be of the same color.  The color is collapsed into the minimal
// expression necessary to change from the color of the last run of text to
// the color of the next run of text.  The initial color state and last color
// state are both CS_NORMAL.
//
// Parsing runs of text: As long as mux_color() returns COLOR_NOTCOLOR, we are
// in a run of text.
//
// Parsing runs of color: As long as mux_color() returns something besides
// COLOR_NOTCOLOR, we are in a run of color code points and use
// UpdateColorState() to merge the color code point into the current color
// state.
//
// It is sometimes not necessary to parse the entire string. Once we truncate
// a run of text, we know that no runs of text after the truncated one will
// fit either.
//
// The initial state is { CS_NORMAL, "" }.  In turn, new runs of text,
// { CS(i), TEXT(i) } are encoded.  To encode { CS(i+1), TEXT(i+1) } on the
// end of { CS(i), TEXT(i) }, we use ColorTransitionBinary(CS(i), CS(i+1), ...)
// and ColorBinaryNormal(CS(i+1), ...) to determine the two transitions.  If
// there isn't enough room remaining in the buffer for these transitions,
// TEXT(i+1), TEXT(i+1) is truncated until there is room.  If TEXT(i+1) is
// truncated to zero, neither color transition nor any of TEXT(i+1) is used.
//
size_t TruncateToBuffer
(
    const UTF8 *pString,
    UTF8 *pBuffer,
    size_t nBuffer
)
{
    size_t nOutput = 0;

    if (  NULL == pBuffer
       || NULL == pString
       || 0 == nBuffer
       || '\0' == pString[0])
    {
        if (NULL != pBuffer)
        {
            pBuffer[0] = '\0';
        }
        return nOutput;
    }

    size_t nNormal;
    const UTF8 *pNormal;

    ColorState csLast    = CS_NORMAL;
    ColorState csCurrent = CS_NORMAL;

    bool bText = false;
    const UTF8 *p = pString;
    bool bTruncated = false;
    while (  '\0' != p[0]
          && !bTruncated)
    {
        // Parse a run of color code points.
        //
        int iCode;
        while (  UTF8_SIZE3 == utf8_FirstByte[p[0]]
              && COLOR_NOTCOLOR != (iCode = mux_color(p)))
        {
            csCurrent = UpdateColorState(csCurrent, iCode);
            p += utf8_FirstByte[p[0]];
        }

        // Parse a run of text.  A run of text is always ended by '\0' and
        // sometimes by '\xEF' since all color code points start with '\xEF'.
        //
        bText = false;
        size_t nTextRun = 0;
        const UTF8 *pTextRun = p;
        for (;;)
        {
            const UTF8 *pEF = (UTF8 *)strchr((char *)p, '\xEF');
            if (NULL == pEF)
            {
                size_t n = strlen((char *)pTextRun);
                nTextRun += n;
                p += n;
                break;
            }
            else
            {
                nTextRun += pEF - p;
                p = pEF;
                if (COLOR_NOTCOLOR != mux_color(p))
                {
                    break;
                }
                nTextRun += UTF8_SIZE3;
                p += UTF8_SIZE3;
            }
        }

        // We have either reached a color code point or end of the string.
        // We have seen { csCurrent, (nTextRun, pTextRun) }.  Before we parse
        // further, we need to encode this into the destination buffer.
        // There is a color transition, then a possibly truncated run of text
        // followed by another color transition to CS_NORMAL.  We won't lay
        // anything down unless there is room for at least one character of
        // the text.
        //
        if (0 < nTextRun)
        {
            // Calculate the two transitions.
            //
            size_t nTransition;
            const UTF8 *pTransition = ColorTransitionBinary(csLast, csCurrent, &nTransition);
            pNormal = ColorBinaryNormal(csCurrent, &nNormal);

            if (nOutput + nTransition + utf8_FirstByte[pTextRun[0]] + nNormal <= nBuffer)
            {
                // Lay down the initial color transition.
                //
                if (0 < nTransition)
                {
                    memcpy(pBuffer + nOutput, pTransition, nTransition);
                    nOutput += nTransition;
                    csLast = csCurrent;
                }

                if (  nBuffer < nOutput + nTextRun + nNormal
                   && nOutput + nNormal <= nBuffer)
                {
                    // We need to truncate the text.
                    //
                    nTextRun = nBuffer - (nOutput + nNormal);
                    while (  0 < nTextRun
                          && UTF8_CONTINUE <= utf8_FirstByte[pTextRun[nTextRun]])
                    {
                        nTextRun--;
                    }
                    bTruncated = true;
                }

                // Lay down text.
                //
                bText = true;
                memcpy(pBuffer + nOutput, pTextRun, nTextRun);
                nOutput += nTextRun;

                // We have left room for the transition to CS_NORMAL, but it
                //  isn't laid down.
                //
            }
        }
    }

    pNormal = ColorBinaryNormal((bText)?csCurrent:csLast, &nNormal);
    if (  0 < nNormal
       && nOutput + nNormal <= nBuffer)
    {
        memcpy(pBuffer + nOutput, pNormal, nNormal);
        nOutput += nNormal;
    }

    pBuffer[nOutput] = '\0';
    return nOutput;
}

mux_field PadField( UTF8 *pBuffer, size_t nMaxBytes, LBUF_OFFSET nMinWidth,
                    mux_field fldOutput)
{
    if (NULL == pBuffer)
    {
        return fldMin;
    }

    while (  fldOutput.m_byte   < nMaxBytes
          && fldOutput.m_column < nMinWidth)
    {
        pBuffer[fldOutput.m_byte] = (UTF8)' ';
        fldOutput += fldAscii;
    }
    if (fldOutput.m_byte <= nMaxBytes)
    {
        pBuffer[fldOutput.m_byte] = '\0';
    }
    return fldOutput;
}

void ItemToList_Init(ITL *p, UTF8 *arg_buff, UTF8 **arg_bufc,
    UTF8 arg_chPrefix, UTF8 arg_chSep)
{
    p->bFirst = true;
    p->chPrefix = arg_chPrefix;
    p->chSep = arg_chSep;
    p->buff = arg_buff;
    p->bufc = arg_bufc;
    p->nBufferAvailable = LBUF_SIZE - (*arg_bufc - arg_buff) - 1;
}

bool ItemToList_AddInteger(ITL *pContext, int i)
{
    UTF8 smbuf[SBUF_SIZE];
    UTF8 *p = smbuf;
    if (  !pContext->bFirst
       && pContext->chSep)
    {
        *p++ = pContext->chSep;
    }
    if (pContext->chPrefix)
    {
        *p++ = pContext->chPrefix;
    }
    p += mux_ltoa(i, p);
    size_t nLen = p - smbuf;
    if (  pContext->nBufferAvailable < nLen
       || sizeof(smbuf) < nLen)
    {
        // Out of room.
        //
        return false;
    }
    if (pContext->bFirst)
    {
        pContext->bFirst = false;
    }
    memcpy(*(pContext->bufc), smbuf, nLen);
    *(pContext->bufc) += nLen;
    pContext->nBufferAvailable -= nLen;
    return true;
}

bool ItemToList_AddInteger64(ITL *pContext, INT64 i64)
{
    UTF8 smbuf[SBUF_SIZE];
    UTF8 *p = smbuf;
    if (  !pContext->bFirst
       && pContext->chSep)
    {
        *p++ = pContext->chSep;
    }
    if (pContext->chPrefix)
    {
        *p++ = pContext->chPrefix;
    }
    p += mux_i64toa(i64, p);
    size_t nLen = p - smbuf;
    if (  pContext->nBufferAvailable < nLen
       || sizeof(smbuf) < nLen)
    {
        // Out of room.
        //
        return false;
    }
    if (pContext->bFirst)
    {
        pContext->bFirst = false;
    }
    memcpy(*(pContext->bufc), smbuf, nLen);
    *(pContext->bufc) += nLen;
    pContext->nBufferAvailable -= nLen;
    return true;
}

bool ItemToList_AddStringLEN(ITL *pContext, size_t nStr, const UTF8 *pStr)
{
    size_t nLen = nStr;
    if (  !pContext->bFirst
       && pContext->chSep)
    {
        nLen++;
    }
    if (pContext->chPrefix)
    {
        nLen++;
    }
    if (nLen > pContext->nBufferAvailable)
    {
        // Out of room.
        //
        return false;
    }
    UTF8 *p = *(pContext->bufc);
    if (pContext->bFirst)
    {
        pContext->bFirst = false;
    }
    else if (pContext->chSep)
    {
        *p++ = pContext->chSep;
    }
    if (pContext->chPrefix)
    {
        *p++ = pContext->chPrefix;
    }
    memcpy(p, pStr, nStr);
    *(pContext->bufc) += nLen;
    pContext->nBufferAvailable -= nLen;
    return true;
}

bool ItemToList_AddString(ITL *pContext, const UTF8 *pStr)
{
    size_t nStr = strlen((const char *)pStr);
    return ItemToList_AddStringLEN(pContext, nStr, pStr);
}

void ItemToList_Final(ITL *pContext)
{
    **(pContext->bufc) = '\0';
}

// mux_stricmp - Compare two strings ignoring case.
//
int mux_stricmp(const UTF8 *a, const UTF8 *b)
{
    while (  *a
          && *b
          && mux_tolower_ascii(*a) == mux_tolower_ascii(*b))
    {
        a++;
        b++;
    }

    int c1 = mux_tolower_ascii(*a);
    int c2 = mux_tolower_ascii(*b);
    if (c1 < c2)
    {
        return -1;
    }
    else if (c1 > c2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// mux_memicmp - Compare two buffers ignoring case.
//
int mux_memicmp(const void *p1_arg, const void *p2_arg, size_t n)
{
    UTF8 *p1 = (UTF8 *)p1_arg;
    UTF8 *p2 = (UTF8 *)p2_arg;
    while (  n
          && mux_tolower_ascii(*p1) == mux_tolower_ascii(*p2))
    {
        n--;
        p1++;
        p2++;
    }
    if (n)
    {
        int c1 = mux_tolower_ascii(*p1);
        int c2 = mux_tolower_ascii(*p2);
        if (c1 < c2)
        {
            return -1;
        }
        else if (c1 > c2)
        {
            return 1;
        }
    }
    return 0;
}


// mux_strlwr - Convert string to all lower case.
//
UTF8 *mux_strlwr(const UTF8 *a, size_t &n)
{
    static UTF8 Buffer[LBUF_SIZE];

    n = 0;
    while ('\0' != *a)
    {
        size_t j;
        size_t m;
        bool bXor;
        const string_desc *qDesc = mux_tolower(a, bXor);
        if (NULL == qDesc)
        {
            m = utf8_FirstByte[Buffer[n]];
            if (LBUF_SIZE-1 < n + m)
            {
                break;
            }

            for (j = 0; j < m; j++)
            {
                Buffer[n+j] = a[j];
            }
        }
        else
        {
            m = qDesc->n_bytes;
            if (LBUF_SIZE-1 < n + m)
            {
                break;
            }

            if (bXor)
            {
                for (j = 0; j < m; j++)
                {
                    Buffer[n+j] = a[j] ^ qDesc->p[j];
                }
            }
            else
            {
                for (j = 0; j < m; j++)
                {
                    Buffer[n+j] = qDesc->p[j];
                }
            }
        }
        n += m;
        a = utf8_NextCodePoint(a);
    }
    Buffer[n] = '\0';
    return Buffer;
}

// mux_strupr - Convert string to all upper case.
//
UTF8 *mux_strupr(const UTF8 *a, size_t &n)
{
    static UTF8 Buffer[LBUF_SIZE];

    n = 0;
    while ('\0' != *a)
    {
        size_t j;
        size_t m;
        bool bXor;
        const string_desc *qDesc = mux_toupper(a, bXor);
        if (NULL == qDesc)
        {
            m = utf8_FirstByte[Buffer[n]];
            if (LBUF_SIZE-1 < n + m)
            {
                break;
            }

            for (j = 0; j < m; j++)
            {
                Buffer[n+j] = a[j];
            }
        }
        else
        {
            m = qDesc->n_bytes;
            if (LBUF_SIZE-1 < n + m)
            {
                break;
            }

            if (bXor)
            {
                for (j = 0; j < m; j++)
                {
                    Buffer[n+j] = a[j] ^ qDesc->p[j];
                }
            }
            else
            {
                for (j = 0; j < m; j++)
                {
                    Buffer[n+j] = qDesc->p[j];
                }
            }
        }
        n += m;
        a = utf8_NextCodePoint(a);
    }
    Buffer[n] = '\0';
    return Buffer;
}

// mux_vsnprintf - Is an sprintf-like function that will not overflow
// a buffer of specific size. The size is give by count, and count
// should be chosen to include the '\0' termination.
//
// Returns: A number from 0 to count-1 that is the string length of
// the returned (possibly truncated) buffer.
//
size_t DCL_CDECL mux_vsnprintf(__in_ecount(nBuffer) UTF8 *pBuffer, __in size_t nBuffer, __in_z const UTF8 *pFmt, va_list va)
{
    if (  NULL == pBuffer
       || nBuffer < 1)
    {
       return 0;
    }
    size_t nLimit = nBuffer-1;

    // Rather than copy a character at a time, some copies are deferred and performed in a single request.
    //
    size_t iFmtDeferred;
    size_t dDeferred = 0;

    size_t iBuffer = 0;
    size_t ncpFmt;
    size_t iFmt = 0;
    if (  NULL != pFmt
       && utf8_strlen(pFmt, ncpFmt))
    {
        static UTF8 Buff[I64BUF_SIZE];

        while (0 != ncpFmt)
        {
            if ('%' != pFmt[iFmt])
            {
                // Ordinary character.
                //
                size_t d = utf8_FirstByte[pFmt[iFmt]];
                size_t dProposed = dDeferred + d;
                if (nLimit < iBuffer + dProposed)
                {
                    if (0 < dDeferred)
                    {
                        // Unravel the deferred copy.
                        //
                        memcpy(pBuffer + iBuffer, pFmt + iFmtDeferred, dDeferred);
                        iBuffer += dDeferred;
                        dDeferred = 0;
                    }
                    goto done;
                }
                else if (0 == dDeferred)
                {
                    iFmtDeferred = iFmt;
                }
                dDeferred = dProposed;

                iFmt += d;
                ncpFmt--;
            }
            else
            {
                if (0 < dDeferred)
                {
                    // Unravel the deferred copy.
                    //
                    memcpy(pBuffer + iBuffer, pFmt + iFmtDeferred, dDeferred);
                    iBuffer += dDeferred;
                    dDeferred = 0;
                }

                size_t cbBuff;
                size_t cpBuff;
                size_t nWidth;
                size_t nPrecision;
                bool bLeft = false;
                bool bZeroPadded = false;
                bool bWidth = false;
                bool bSawPeriod = false;
                bool bPrecision = false;
                int nLongs = 0;

                iFmt++;
                ncpFmt--;

                while (0 != ncpFmt)
                {
                    if (  'd' == pFmt[iFmt]
                       || 's' == pFmt[iFmt]
                       || 'u' == pFmt[iFmt]
                       || 'x' == pFmt[iFmt]
                       || 'X' == pFmt[iFmt]
                       || 'p' == pFmt[iFmt])
                    {
                        UTF8 *pBuff = Buff;

                        if ('d' == pFmt[iFmt])
                        {
                            // Obtain and validate argument.
                            //
                            if (0 == nLongs)
                            {
                                int i = va_arg(va, int);
                                cbBuff = cpBuff = mux_ltoa(i, Buff);
                            }
                            else if (1 == nLongs)
                            {
                                long int i = va_arg(va, long int);
                                cbBuff = cpBuff = mux_ltoa(i, Buff);
                            }
                            else if (2 == nLongs)
                            {
                                INT64 i = va_arg(va, INT64);
                                cbBuff = cpBuff = mux_i64toa(i, Buff);
                            }
                            else
                            {
                                goto done;
                            }
                        }
                        else  if ('s' == pFmt[iFmt])
                        {
                            // Obtain and validate argument.
                            //
                            pBuff = va_arg(va, UTF8 *);
                            if (  !utf8_strlen(pBuff, cpBuff)
                               || 0 != nLongs)
                            {
                                goto done;
                            }
                            cbBuff = strlen((char *)pBuff);

                            if (  bPrecision
                               && nPrecision < cpBuff)
                            {
                                // Need to walk cbBuff back to correspond to changes in cpBuff.
                                //
                                while (cpBuff != nPrecision)
                                {
                                    do
                                    {
                                        cbBuff--;
                                    } while (UTF8_CONTINUE <= utf8_FirstByte[pBuff[cbBuff]]);
                                    cpBuff--;
                                }
                            }
                        }
                        else if ('p' == pFmt[iFmt])
                        {
                            if (  0 != nLongs
                               || bWidth)
                            {
                                goto done;
                            }

                            // Convert pointer to unsigned integer.
                            //
                            union
                            {
                                UINT_PTR ui;
                                void *pv;
                            } u;
                            u.pv = va_arg(va, void *);
#if SIZEOF_UINT_PTR <= SIZEOF_UNSIGNED_LONG
                            cbBuff = cpBuff = mux_utox(u.ui, Buff, true);
#elif SIZEOF_UINT_PTR <= SIZEOF_UNSIGNED_LONG_LONG
                            cbBuff = cpBuff = mux_ui64tox(u.ui, Buff, true);
#else
#error Size of pointer is larger size of largest known integer.
#endif
                            bWidth = true;
                            nWidth = 2*sizeof(UINT_PTR);
                            bZeroPadded = true;
                        }
                        else
                        {
                            bool bHex = (  'x' == pFmt[iFmt]
                                        || 'X' == pFmt[iFmt]);
                            bool bUpper = ('X' == pFmt[iFmt]);

                            // Obtain and validate argument.
                            //
                            if (0 == nLongs)
                            {
                                unsigned int ui = va_arg(va, unsigned int);
                                cbBuff = cpBuff = bHex?mux_utox(ui, Buff, bUpper):mux_utoa(ui, Buff);
                            }
                            else if (1 == nLongs)
                            {
                                unsigned long int ui = va_arg(va, unsigned long int);
                                cbBuff = cpBuff = bHex?mux_utox(ui, Buff, bUpper):mux_utoa(ui, Buff);
                            }
                            else if (2 == nLongs)
                            {
                                UINT64 ui = va_arg(va, UINT64);
                                cbBuff = cpBuff = bHex?mux_ui64tox(ui, Buff, bUpper):mux_ui64toa(ui, Buff);
                            }
                            else
                            {
                                goto done;
                            }
                        }

                        // Calculate and validate needed size.  Numberic and
                        // string fields are at least the size of their width.
                        // String fields may have been truncated above by
                        // precision.
                        //
                        // Width is compared with the number of code points.
                        // Padding is always done with space or zero.
                        //
                        size_t nUsed = cbBuff;
                        size_t nPadding = 0;
                        if (  bWidth
                           && cpBuff < nWidth)
                        {
                            nPadding = nWidth - cpBuff;
                            nUsed += nPadding;
                        }

                        if (nLimit < iBuffer + nUsed)
                        {
                            goto done;
                        }

                        // Apply leading padding if necessary.
                        //
                        if (  !bLeft
                           && bWidth)
                        {
                            if (  'd' == pFmt[iFmt]
                               && '-' == pBuff[0]
                               && 0 < nPadding
                               && bZeroPadded)
                            {
                                // The leading minus sign must be laid down before zero-padding begins.
                                //
                                pBuffer[iBuffer] = '-';
                                iBuffer++;

                                pBuff++;
                                cbBuff--;
                                cpBuff--;
                            }

                            while (0 < nPadding)
                            {
                                pBuffer[iBuffer] = bZeroPadded?'0':' ';
                                iBuffer++;
                                nPadding--;
                            }
                        }

                        // Apply string.
                        //
                        memcpy(pBuffer + iBuffer, pBuff, cbBuff);
                        iBuffer += cbBuff;

                        // Apply trailing padding if necessary.
                        //
                        if (  bLeft
                           && bWidth) 
                        {
                            while (0 < nPadding)
                            {
                                pBuffer[iBuffer] = bZeroPadded?'0':' ';
                                iBuffer++;
                                nPadding--;
                            }
                        }

                        iFmt++;
                        ncpFmt--;
                        break;
                    }
                    else if ('l' == pFmt[iFmt])
                    {
                        nLongs++;
                        iFmt++;
                        ncpFmt--;
                    }
                    else if (  '0' <= pFmt[iFmt]
                            && pFmt[iFmt] <= '9')
                    {
                        if (!bSawPeriod)
                        {
                            if (!bWidth)
                            {
                                if ('0' == pFmt[iFmt])
                                {
                                    if (bZeroPadded)
                                    {
                                        goto done;
                                    }
                                    bZeroPadded = true;
                                }
                                else
                                {
                                    nWidth = pFmt[iFmt] - '0';
                                    bWidth = true;
                                }
                            }
                            else
                            {
                                nWidth = 10 * nWidth + pFmt[iFmt] - '0';
                            }
                        }
                        else
                        {
                            if (!bPrecision)
                            {
                                nPrecision = pFmt[iFmt] - '0';
                                bPrecision = true;
                            }
                            else
                            {
                                nPrecision = 10 * nPrecision + pFmt[iFmt] - '0';
                            }
                        }

                        iFmt++;
                        ncpFmt--;
                    }
                    else if ('.' == pFmt[iFmt])
                    {
                        bSawPeriod = true;

                        iFmt++;
                        ncpFmt--;
                    }
                    else if ('-' == pFmt[iFmt])
                    {
                        if (bLeft)
                        {
                            goto done;
                        }
                        bLeft = true;

                        iFmt++;
                        ncpFmt--;
                    }
                    else if ('c' == pFmt[iFmt])
                    {
                        unsigned int ch = va_arg(va, unsigned int);
                        if (nLimit < iBuffer + 1)
                        {
                            goto done;
                        }
                        pBuffer[iBuffer] = ch;
                        iBuffer++;

                        iFmt++;
                        ncpFmt--;
                        break;
                    }
                    else if ('%' == pFmt[iFmt])
                    {
                        // "%%"
                        //
                        if (nLimit < iBuffer + 1)
                        {
                            goto done;
                        }
                        pBuffer[iBuffer] = '%';
                        iBuffer++;

                        iFmt++;
                        ncpFmt--;
                        break;
                    }
                    else
                    {
                        mux_assert(0);

                        iFmt += utf8_FirstByte[pFmt[iFmt]];
                        ncpFmt--;
                    }
                }
            }
        }

        if (0 < dDeferred)
        {
            // Unravel the deferred copy.
            //
            memcpy(pBuffer + iBuffer, pFmt + iFmtDeferred, dDeferred);
            iBuffer += dDeferred;
            dDeferred = 0;
        }
    }

done:
    pBuffer[iBuffer] = '\0';
    return iBuffer;
}

void DCL_CDECL mux_sprintf(__in_ecount(count) UTF8 *buff, __in size_t count, __in_z const UTF8 *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    (void)mux_vsnprintf(buff, count, fmt, ap);
    va_end(ap);
}

void DCL_CDECL mux_fprintf(FILE *fp, __in_z const UTF8 *fmt, ...)
{
    if (NULL != fp)
    {
        UTF8 Buffer[MBUF_SIZE];

        va_list ap;
        va_start(ap, fmt);
        size_t nBuffer = mux_vsnprintf(Buffer, sizeof(Buffer), fmt, ap);
        va_end(ap);

        fwrite(Buffer, 1, nBuffer, fp);
    }
}

// This function acts like fgets except that any data on the end of the
// line past the buffer size is truncated instead of being returned on
// the next call.
//
size_t GetLineTrunc(UTF8 *Buffer, size_t nBuffer, FILE *fp)
{
    size_t lenBuffer = 0;
    if (fgets((char *)Buffer, static_cast<int>(nBuffer), fp))
    {
        lenBuffer = strlen((char *)Buffer);
    }
    if (lenBuffer <= 0)
    {
        memcpy(Buffer, "\n", 2);
        return 1;
    }
    if (Buffer[lenBuffer-1] != '\n')
    {
        // The line was too long for the buffer. Continue reading until the
        // end of the line.
        //
        UTF8 TruncBuffer[SBUF_SIZE];
        size_t lenTruncBuffer;
        do
        {
            if (!fgets((char *)TruncBuffer, sizeof(TruncBuffer), fp))
            {
                break;
            }
            lenTruncBuffer = strlen((char *)TruncBuffer);
        }
        while (TruncBuffer[lenTruncBuffer-1] != '\n');
    }
    return lenBuffer;
}

// Method: Boyer-Moore-Horspool
//
// This method is a simplification of the Boyer-Moore String Searching
// Algorithm, but a useful one. It does not require as much temporary
// storage, and the setup costs are not as high as the full Boyer-Moore.
//
// If we were searching megabytes of data instead of 8KB at most, then
// the full Boyer-Moore would make more sense.
//
#define BMH_LARGE 32767
void BMH_Prepare(BMH_State *bmhs, size_t nPat, const UTF8 *pPat)
{
    if (nPat <= 0)
    {
        return;
    }
    size_t k;
    for (k = 0; k < 256; k++)
    {
        bmhs->m_d[k] = nPat;
    }

    UTF8 chLastPat = pPat[nPat-1];
    bmhs->m_skip2 = nPat;
    for (k = 0; k < nPat - 1; k++)
    {
        bmhs->m_d[(unsigned char)pPat[k]] = nPat - k - 1;
        if (pPat[k] == chLastPat)
        {
            bmhs->m_skip2 = nPat - k - 1;
        }
    }
    bmhs->m_d[(unsigned char)chLastPat] = BMH_LARGE;
}

bool BMH_Execute(BMH_State *bmhs, size_t *pnMatched, size_t nPat, const UTF8 *pPat, size_t nSrc, const UTF8 *pSrc)
{
    if (nPat <= 0)
    {
        return false;
    }
    for (size_t i = nPat-1; i < nSrc; i += bmhs->m_skip2)
    {
        while ((i += bmhs->m_d[(unsigned char)(pSrc[i])]) < nSrc)
        {
            ; // Nothing.
        }
        if (i < BMH_LARGE)
        {
            break;
        }
        i -= BMH_LARGE;
        int j = static_cast<int>(nPat - 1);
        const UTF8 *s = pSrc + (i - j);
        while (--j >= 0 && s[j] == pPat[j])
        {
            ; // Nothing.
        }
        if (j < 0)
        {
            *pnMatched = s-pSrc;
            return true;
        }
    }
    return false;
}

bool BMH_StringSearch(size_t *pnMatched, size_t nPat, const UTF8 *pPat, size_t nSrc, const UTF8 *pSrc)
{
    BMH_State bmhs;
    BMH_Prepare(&bmhs, nPat, pPat);
    return BMH_Execute(&bmhs, pnMatched, nPat, pPat, nSrc, pSrc);
}

void BMH_PrepareI(BMH_State *bmhs, size_t nPat, const UTF8 *pPat)
{
    if (nPat <= 0)
    {
        return;
    }
    size_t k;
    for (k = 0; k < 256; k++)
    {
        bmhs->m_d[k] = nPat;
    }

    UTF8 chLastPat = pPat[nPat-1];
    bmhs->m_skip2 = nPat;
    for (k = 0; k < nPat - 1; k++)
    {
        bmhs->m_d[mux_toupper_ascii(pPat[k])] = nPat - k - 1;
        bmhs->m_d[mux_tolower_ascii(pPat[k])] = nPat - k - 1;
        if (mux_toupper_ascii(pPat[k]) == mux_toupper_ascii(chLastPat))
        {
            bmhs->m_skip2 = nPat - k - 1;
        }
    }
    bmhs->m_d[mux_toupper_ascii(chLastPat)] = BMH_LARGE;
    bmhs->m_d[mux_tolower_ascii(chLastPat)] = BMH_LARGE;
}

bool BMH_ExecuteI(BMH_State *bmhs, size_t *pnMatched, size_t nPat, const UTF8 *pPat, size_t nSrc, const UTF8 *pSrc)
{
    if (nPat <= 0)
    {
        return false;
    }
    for (size_t i = nPat-1; i < nSrc; i += bmhs->m_skip2)
    {
        while ((i += bmhs->m_d[(unsigned char)(pSrc[i])]) < nSrc)
        {
            ; // Nothing.
        }
        if (i < BMH_LARGE)
        {
            break;
        }
        i -= BMH_LARGE;
        int j = static_cast<int>(nPat - 1);
        const UTF8 *s = pSrc + (i - j);
        while (  --j >= 0
              && mux_toupper_ascii(s[j]) == mux_toupper_ascii(pPat[j]))
        {
            ; // Nothing.
        }
        if (j < 0)
        {
            *pnMatched = s-pSrc;
            return true;
        }
    }
    return false;
}

bool BMH_StringSearchI(size_t *pnMatched, size_t nPat, const UTF8 *pPat, size_t nSrc, const UTF8 *pSrc)
{
    BMH_State bmhs;
    BMH_PrepareI(&bmhs, nPat, pPat);
    return BMH_ExecuteI(&bmhs, pnMatched, nPat, pPat, nSrc, pSrc);
}

// ---------------------------------------------------------------------------
// cf_art_except:
//
// Add an article rule to the ruleset.
//

CF_HAND(cf_art_rule)
{
    UNUSED_PARAMETER(pExtra);
    UNUSED_PARAMETER(nExtra);

    UTF8 *pCurrent = str;

    while (mux_isspace(*pCurrent))
    {
        pCurrent++;
    }
    UTF8 *pArticle = pCurrent;
    while (  !mux_isspace(*pCurrent)
          && *pCurrent != '\0')
    {
        pCurrent++;
    }
    if (*pCurrent == '\0')
    {
        cf_log_syntax(player, cmd, T("No article or regexp specified."));
        return -1;
    }

    bool bUseAn = false;
    bool bOkay = false;

    if (pCurrent - pArticle <= 2)
    {
        if (  'a' == pArticle[0]
           || 'A' == pArticle[0])
        {
            if (  'n' == pArticle[1]
               || 'N' == pArticle[1])
            {
                bUseAn = true;
                bOkay = true;
            }

            if (mux_isspace(pArticle[1]))
            {
                bOkay = true;
            }
        }
    }

    if (!bOkay)
    {
        *pCurrent = '\0';
        cf_log_syntax(player, cmd, T("Invalid article \xE2\x80\x98%s\xE2\x80\x99."), pArticle);
        return -1;
    }

    while (mux_isspace(*pCurrent))
    {
        pCurrent++;
    }

    if (*pCurrent == '\0')
    {
        cf_log_syntax(player, cmd, T("No regexp specified."));
        return -1;
    }

    const char *errptr;
    int erroffset;
    pcre* reNewRegexp = pcre_compile((char *)pCurrent, PCRE_UTF8, &errptr, &erroffset, NULL);
    if (!reNewRegexp)
    {
        cf_log_syntax(player, cmd, T("Error processing regexp \xE2\x80\x98%s\xE2\x80\x99:."),
              pCurrent, errptr);
        return -1;
    }

    pcre_extra *study = pcre_study(reNewRegexp, 0, &errptr);

    ArtRuleset** arRules = (ArtRuleset **) vp;

    ArtRuleset* arNewRule = NULL;
    try
    {
        arNewRule = new ArtRuleset;
    }
    catch (...)
    {
        ; // Nothing.
    }

    if (NULL != arNewRule)
    {
        // Push new rule at head of list.
        //
        arNewRule->m_pNextRule = *arRules;
        arNewRule->m_bUseAn = bUseAn;
        arNewRule->m_pRegexp = reNewRegexp;
        arNewRule->m_pRegexpStudy = study;
        *arRules = arNewRule;
    }
    else
    {
        MEMFREE(reNewRegexp);
        if (study)
        {
            MEMFREE(study);
        }
        cf_log_syntax(player, cmd, T("Out of memory."));
        return -1;
    }

    return 0;
}

/*! \brief Constructs mux_string object.
 *
 * This constructor puts the mux_string object into an initial, reasonable,
 * and empty state.
 *
 * \return         None.
 */

mux_string::mux_string(void)
{
    m_iLast = CursorMin;
    m_autf[0] = '\0';
    m_ncs = 0;
    m_pcs = NULL;
}

/*! \brief Constructs mux_string object.
 *
 * This is a deep copy constructor.
 *
 * \param sStr     mux_string to be copied.
 * \return         None.
 */

mux_string::mux_string(const mux_string &sStr)
{
    m_iLast = CursorMin;
    m_autf[0] = '\0';
    m_ncs = 0;
    m_pcs = NULL;
    import(sStr);
}

/*! \brief Constructs mux_string object from an ANSI string.
 *
 * Parses the given ANSI string into a form which can be more-easily
 * navigated.
 *
 * \param pStr     ANSI string to be parsed.
 * \return         None.
 */

mux_string::mux_string(const UTF8 *pStr)
{
    m_iLast = CursorMin;
    m_autf[0] = '\0';
    m_ncs = 0;
    m_pcs = NULL;
    import(pStr);
}

/*! \brief Destructs mux_string object.
 *
 * This destructor deletes the m_pcs array if necessary.
 *
 * \return         None.
 */

mux_string::~mux_string(void)
{
    realloc_m_pcs(0);
}

/*! \brief Self-checks mux_string to validate the invariant.
 *
 * Used in debugging to validate that a string is internally consistent and maintains the described invariants.
 *
 * \return         None.
 */

void mux_string::Validate(void) const
{
    // m_iLast.m_byte is always between 0 and LBUF_SIZE-1 inclusively.
    //
    mux_assert(m_iLast.m_byte <= LBUF_SIZE-1);

    // m_iLast.m_point is always between 0 and LBUF_SIZE-1 inclusively.
    //
    mux_assert(m_iLast.m_point <= LBUF_SIZE-1);

    // m_iLast.m_point is always between 0 and LBUF_SIZE-1 inclusively.
    //
    mux_assert(m_ncs <= LBUF_SIZE-1);

    // When m_pcs is NULL, m_ncs must be 0, and this is equivalent to every
    // code point having CS_NORMAL color.  When m_pcs is not NULL, m_ncs
    // must be between 1 and LBUF_SIZE-1, inclusively, and it must be large
    // enough to contain colors for every code point.
    //
    mux_assert(  ( NULL == m_pcs
                 && 0 == m_ncs)
              || ( NULL != m_pcs
                 && 1 <= m_ncs
                 && m_ncs <= LBUF_SIZE-1
                 && m_iLast.m_point <= m_ncs));

    // m_iLast.m_byte bytes of m_atuf[] contain the non-color UTF-8-encoded
    // code points.  A terminating '\0' at m_autf[m_iLast.m_byte] is not
    // included in this size even though '\0' is a UTF-8 code point.  In this
    // way, m_iLast.m_byte corresponds to strlen() in units of bytes.
    //
    // m_iLast.m_point represents the number of non-color UTF-8-encoded code
    // points stored in m_autf[]. The terminating '\0' is not included in this
    // size.  In this way, m_ncp corresponds to strlen() in units of code
    // points.
    //
    size_t nbytes  = 0;
    size_t npoints = 0;

    const UTF8 *p = m_autf;
    while (  p < (m_autf + LBUF_SIZE)
          && '\0' != *p)
    {
        // Each code point must be valid encoding.
        //
        unsigned char iCode = utf8_FirstByte[*p];
        mux_assert(iCode < UTF8_CONTINUE);

        size_t j;
        for (j = 1; j < iCode; j++)
        {
            mux_assert(UTF8_CONTINUE == utf8_FirstByte[p[j]]);
        }

        // All code points in m_autf[] must be non-color code points.
        //
        mux_assert(COLOR_NOTCOLOR == mux_color(p));

        p += iCode;
        nbytes += iCode;
        npoints++;
    }

    mux_assert(nbytes == m_iLast.m_byte);
    mux_assert(npoints == m_iLast.m_point);

    if (NULL != m_pcs)
    {
        // Every ColorState must be valid.
        //
        size_t i;
        for (i = 0; i < m_iLast.m_point; i++)
        {
            ValidateColorState(m_pcs[i]);
        }
    }
}

void mux_string::append(dbref num)
{
    append_TextPlain(T("#"), 1);
    append_TextPlain(mux_ltoa_t(num));
}

void mux_string::append(INT64 iInt)
{
    append_TextPlain(mux_i64toa_t(iInt));
}

void mux_string::append(long lLong)
{
    append_TextPlain(mux_ltoa_t(lLong));
}

/*! \brief Extract and append a range of characters.
 *
 * \param sStr     mux_string from which to extract characters.
 * \param nStart   Beginning of range to extract and apend.
 * \param iEnd     End of range.
 * \return         None.
 */

void mux_string::append(const mux_string &sStr, mux_cursor iStart, mux_cursor iEnd)
{
    if (  sStr.m_iLast <= iStart
       || iEnd <= iStart
       || LBUF_SIZE-1 == m_iLast.m_byte)
    {
        // The selection range is empty, or no buffer space is left.
        //
        return;
    }

    if (sStr.m_iLast < iEnd)
    {
        iEnd = sStr.m_iLast;
    }

    if (CursorMax - m_iLast < iEnd - iStart)
    {
        while (LBUF_SIZE-1 < m_iLast.m_byte + iEnd.m_byte - iStart.m_byte)
        {
            sStr.cursor_prev(iEnd);
        }
    }

    LBUF_OFFSET nBytes = iEnd.m_byte - iStart.m_byte;
    LBUF_OFFSET nPoints = iEnd.m_point - iStart.m_point;

    if (  0 != m_ncs
       || 0 != sStr.m_ncs)
    {
        // One of the strings specifies ColorStates, so the result will need
        // to as well.
        //
        realloc_m_pcs(m_iLast.m_point + nPoints);
    }

    memcpy(m_autf + m_iLast.m_byte, sStr.m_autf + iStart.m_byte, nBytes);

    if (0 != sStr.m_ncs)
    {
        memcpy(m_pcs + m_iLast.m_point, sStr.m_pcs + iStart.m_point, nPoints * sizeof(m_pcs[0]));
    }
    else if (0 != m_ncs)
    {
        for (size_t i = 0; i < nPoints; i++)
        {
            m_pcs[m_iLast.m_point + i] = CS_NORMAL;
        }
    }

    m_iLast(m_iLast.m_byte + nBytes, m_iLast.m_point + nPoints);
    m_autf[m_iLast.m_byte] = '\0';
}

void mux_string::append(const UTF8 *pStr)
{
    if (  NULL == pStr
       || '\0' == *pStr)
    {
        return;
    }

    size_t nAvail = (LBUF_SIZE-1) - m_iLast.m_byte;
    if (0 == nAvail)
    {
        // No room.
        //
        return;
    }

    size_t nLen = strlen((char *)pStr);
    if (nAvail < nLen)
    {
        nLen = nAvail;
    }

    mux_string *sNew = new mux_string;

    sNew->import(pStr, nLen);

    append(*sNew);
    delete sNew;
}

void mux_string::append(const UTF8 *pStr, size_t nLen)
{
    if (  NULL == pStr
       || '\0' == *pStr)
    {
        return;
    }

    size_t nAvail = (LBUF_SIZE-1) - m_iLast.m_byte;
    if (0 == nAvail)
    {
        // No room.
        //
        return;
    }
    if (nAvail < nLen)
    {
        nLen = nAvail;
    }

    mux_string *sNew = new mux_string;

    sNew->import(pStr, nLen);
    append(*sNew);
    delete sNew;
}

void mux_string::append_TextPlain(const UTF8 *pStr)
{
    if (  '\0' == *pStr
       || LBUF_SIZE-1 <= m_iLast.m_byte)
    {
        // The selection range is empty, or no buffer space is left.
        //
        return;
    }

    size_t nLen = strlen((char *)pStr);

    if (static_cast<size_t>((LBUF_SIZE-1) - m_iLast.m_byte) < nLen)
    {
        nLen = (LBUF_SIZE-1) - m_iLast.m_byte;
    }

    memcpy(m_autf + m_iLast.m_byte, pStr, nLen * sizeof(m_autf[0]));

    mux_cursor i = m_iLast, j = i;
    while (  cursor_next(i)
          && i.m_byte <= m_iLast.m_byte + nLen)
    {
        j = i;
    }

    if (0 != m_ncs)
    {
        // Note that mux_string invariant is not maintained on the following
        // call to realloc_m_pcs().
        //
        realloc_m_pcs(j.m_point);
        for (size_t k = m_iLast.m_point; k < j.m_point; k++)
        {
            m_pcs[k] = CS_NORMAL;
        }
    }

    m_iLast = j;
    m_autf[m_iLast.m_byte] = '\0';
}

void mux_string::append_TextPlain(const UTF8 *pStr, size_t nLen)
{
    if (  '\0' == *pStr
       || 0 == nLen
       || LBUF_SIZE-1 == m_iLast.m_byte)
    {
        // The selection range is empty, or no buffer space is left.
        //
        return;
    }

    if (static_cast<size_t>((LBUF_SIZE-1) - m_iLast.m_byte) < nLen)
    {
        nLen = (LBUF_SIZE-1) - m_iLast.m_byte;
    }

    if (0 != m_ncs)
    {
        realloc_m_pcs(m_iLast.m_point + nLen);
        for (size_t i = 0; i < nLen; i++)
        {
            m_pcs[m_iLast.m_point + i] = CS_NORMAL;
        }
    }

    memcpy(m_autf + m_iLast.m_byte, pStr, nLen * sizeof(m_autf[0]));

    mux_cursor i = m_iLast, j = i;
    while (  cursor_next(i)
          && i.m_byte <= m_iLast.m_byte + nLen)
    {
        j = i;
    }

    m_iLast = j;
    m_autf[m_iLast.m_byte] = '\0';
}

/*! \brief Compress each run of consecutive occurrences of the
 * specified character to a single character.
 *
 * For each compressed run of characters, the properties of the
 * first character are preserved.
 *
 * \param ch       Character to compress.
 * \return         None.
 */

void mux_string::compress(const UTF8 *ch)
{
    mux_cursor i, j;
    cursor_start(i);
    LBUF_OFFSET nChar = utf8_FirstByte[ch[0]];
    LBUF_OFFSET k;

    while (i < m_iLast)
    {
        if (m_autf[i.m_byte] == ch[0])
        {
            k = 1;
            while (  i.m_byte + k < m_iLast.m_byte
                  && m_autf[i.m_byte + k] == ch[k % nChar])
            {
                k++;
            }
            if (1 < k)
            {
                j = i;
                while (  j.m_byte < i.m_byte + k
                      && cursor_next(j))
                {
                    ; // Nothing.
                };
                if (i.m_byte + k < j.m_byte)
                {
                    cursor_prev(j);
                }
                cursor_next(i);
                delete_Chars(i, j);
            }
        }
        cursor_next(i);
    }
}

/*! \brief Compress each run of consecutive whitespace characters to a
 * single whitespace character.
 *
 * For each compressed run of whitespace characters, the properties
 * of the first whitespace character are preserved.
 *
 * \return         None.
 */

void mux_string::compress_Spaces(void)
{
    mux_cursor i = CursorMin, j = CursorMin;
    while (i < m_iLast)
    {
        if (mux_isspace(m_autf[i.m_byte]))
        {
            // look ahead for consecutive whitespace characters
            //
            j = i;
            while (  cursor_next(j)
                  && mux_isspace(m_autf[j.m_byte]))
            {
                ; // Nothing.
            }
            if (  cursor_next(i)
               && i < j)
            {
                delete_Chars(i, j);
            }
        }
    }
    cursor_next(i);
}

/*! \brief Delete a range of characters.
 *
 * \param nStart   Beginning of range to delete.
 * \param iEnd     End of range.
 * \return         None.
 */

void mux_string::delete_Chars(mux_cursor iStart, mux_cursor iEnd)
{
    if (  m_iLast <= iStart
       || iEnd <= iStart)
    {
        // The range does not select any characters.
        //
        return;
    }

    if (m_iLast <= iEnd)
    {
        // The range extends beyond the end, so we can simply truncate.
        //
        m_iLast = iStart;
        m_autf[m_iLast.m_byte] = '\0';
        return;
    }

    size_t nBytesMove = m_iLast.m_byte - iEnd.m_byte;
    size_t nPointsMove = m_iLast.m_point - iEnd.m_point;
    memmove(m_autf + iStart.m_byte, m_autf + iEnd.m_byte, nBytesMove);
    if (0 != m_ncs)
    {
        memmove(m_pcs + iStart.m_point, m_pcs + iEnd.m_point, nPointsMove * sizeof(m_pcs[0]));
    }
    m_iLast(m_iLast.m_byte - (iEnd.m_byte - iStart.m_byte), m_iLast.m_point - (iEnd.m_point - iStart.m_point));
    m_autf[m_iLast.m_byte] = '\0';
}

/*! \brief Perform a search-and-replace-all operation, or prepend or
 * append the replacement string.
 *
 * If sFrom is "^", sTo is prepended to this string. If sFrom is "$",
 * sTo is appended to this string.  Search-and-replace-all for a literal
 * caret or dollar sign can be done by escaping them with a backslash or
 * percent sign, but in that case sFrom will be destructively modified
 * to remove the escape character.
 *
 * \param sFrom     String to search for within this string.
 * \param sTo       Replacement string.
 * \return          None.
 */

void mux_string::edit(mux_string &sFrom, const mux_string &sTo)
{
    // Do the substitution.  Idea for prefix/suffix from R'nice@TinyTIM.
    //
    mux_cursor nFrom = sFrom.m_iLast;
    if (  1 == nFrom.m_byte
       && '^' == sFrom.m_autf[0])
    {
        // Prepend 'to' to string.
        //
        prepend(sTo);
    }
    else if (  1 == nFrom.m_byte
            && '$' == sFrom.m_autf[0])
    {
        // Append 'to' to string.
        //
        append(sTo);
    }
    else
    {
        // Replace all occurances of 'from' with 'to'. Handle the special
        // cases of from = \$ and \^.
        //
        if (  (  '\\' == sFrom.m_autf[0]
              || '%' == sFrom.m_autf[0])
           && (  '$' == sFrom.m_autf[1]
              || '^' == sFrom.m_autf[1])
           && 2 == nFrom.m_byte)
        {
            mux_cursor n(1, 1);
            sFrom.delete_Chars(CursorMin, n);
            nFrom(nFrom.m_byte-1, nFrom.m_point-1);
        }

        mux_cursor iStart = CursorMin, iFound = CursorMin;
        mux_cursor nTo = sTo.m_iLast;
        bool bSucceeded = search(sFrom, &iFound);
        while (bSucceeded)
        {
            iStart = iFound;
            replace_Chars(sTo, iStart, nFrom);
            iStart = iStart + nTo;

            if (iStart < m_iLast)
            {
                bSucceeded = search(sFrom, &iFound, iStart);
            }
            else
            {
                bSucceeded = false;
            }
        }
    }
}

// This function is deprecated.
//
UTF8 mux_string::export_Char(size_t n) const
{
    if (m_iLast.m_byte <= n)
    {
        return '\0';
    }
    return m_autf[n];
}

LBUF_OFFSET mux_string::export_Char_UTF8(size_t iFirst, UTF8 *pBuffer) const
{
    if (m_iLast.m_byte <= iFirst)
    {
        if (NULL != pBuffer)
        {
            pBuffer[0] = '\0';
        }
        return 0;
    }

    LBUF_OFFSET nBytes = utf8_FirstByte[m_autf[iFirst]];

    if (UTF8_CONTINUE <= nBytes)
    {
        if (NULL != pBuffer)
        {
            pBuffer[0] = '\0';
        }
        return 0;
    }

    if (NULL != pBuffer)
    {
        LBUF_OFFSET i;
        for (i = 0; i < nBytes; i++)
        {
            pBuffer[i] = m_autf[iFirst + i];
        }
        pBuffer[i] = '\0';
    }
    return nBytes;
}

ColorState mux_string::export_Color(size_t n) const
{
    if (  m_iLast.m_point <= n
       || 0 == m_ncs)
    {
        return CS_NORMAL;
    }
    ValidateColorState(m_pcs[n]);
    return m_pcs[n];
}

double mux_string::export_Float(bool bStrict) const
{
    return mux_atof(m_autf, bStrict);
}

INT64 mux_string::export_I64(void) const
{
    return mux_atoi64(m_autf);
}

long mux_string::export_Long(void) const
{
    return mux_atol(m_autf);
}

/*! \brief Generates colored string from internal form.
 *
 * \param buff      Pointer to beginning of lbuf.
 * \param bufc      Pointer to current position. Defaults to NULL.
 * \param iStart    String position to begin copying from. Defaults to (0, 0).
 * \param iEnd      Last string position to copy. Defaults to (LBUF_SIZE-1, LBUF_SIZE-1).
 * \param nBytesMax Size of buffer we're outputting into.
 *                  Defaults to LBUF_SIZE-1.
 * \return          Number of bytes copied.
 */

LBUF_OFFSET mux_string::export_TextColor
(
    UTF8 *pBuffer,
    mux_cursor iStart,
    mux_cursor iEnd,
    size_t nBytesMax
) const
{
    // Sanity check our arguments and find out how much room we have.
    // We assume we're outputting into an LBUF unless given a smaller nBuffer.
    //
    if (!pBuffer)
    {
        return 0;
    }
    if (  m_iLast <= iStart
       || iEnd <= iStart
       || 0 == nBytesMax)
    {
        *pBuffer = '\0';
        return 0;
    }
    if (m_iLast < iEnd)
    {
        iEnd = m_iLast;
    }

    // iStart is the position in the source string where we will start
    //  copying, and has a value in the range [0, m_iLast).
    // nAvail is the room left in the destination buffer,
    //  and has a value in the range (0, nBuffer).
    // nLeft is the length of the portion of the source string we'd
    //  like to copy, and has a value in the range (0, m_iLast.m_byte].
    // nLen is the length of the portion of the source string we will
    //  try to copy, and has a value in the ranges (0, nLeft] and (0, nAvail].
    //
    LBUF_OFFSET nBytesWanted  = iEnd.m_byte - iStart.m_byte;
    LBUF_OFFSET nPointsWanted = iEnd.m_point - iStart.m_point;
    if (0 == m_ncs)
    {
        return export_TextPlain(pBuffer, iStart, iEnd, nBytesMax);
    }
    bool bPlentyOfRoom = (nBytesMax >
        static_cast<size_t>(nBytesWanted + (COLOR_MAXIMUM_BINARY_TRANSITION_LENGTH * nPointsWanted) + COLOR_MAXIMUM_BINARY_NORMAL + 1));
    mux_cursor iPos = iStart, iCopy = iStart;
    size_t nTransition = 0;
    const UTF8 *pTransition = NULL;
    ColorState csPrev = CS_NORMAL;
    LBUF_OFFSET nCopy = 0, nDone = 0;

    if (bPlentyOfRoom)
    {
        while (iPos < iEnd)
        {
            if (csPrev != m_pcs[iPos.m_point])
            {
                if (iCopy < iPos)
                {
                    nCopy = iPos.m_byte - iCopy.m_byte;
                    memcpy(pBuffer + nDone, m_autf + iCopy.m_byte, nCopy);
                    nDone += nCopy;
                    iCopy = iPos;
                }

                pTransition = ColorTransitionBinary( csPrev,
                                                     m_pcs[iPos.m_point],
                                                     &nTransition);
                memcpy(pBuffer + nDone, pTransition, nTransition * sizeof(pTransition[0]));
                nDone += nTransition;
                csPrev = m_pcs[iPos.m_point];
            }
            cursor_next(iPos);
        }
        if (iCopy < iPos)
        {
            nCopy = iPos.m_byte - iCopy.m_byte;
            memcpy(pBuffer + nDone, m_autf + iCopy.m_byte, nCopy);
            nDone += nCopy;
        }
        if (csPrev != CS_NORMAL)
        {
            pTransition = ColorBinaryNormal(csPrev, &nTransition);
            memcpy(pBuffer + nDone, pTransition, nTransition * sizeof(pTransition[0]));
            nDone += nTransition;
        }
        pBuffer[nDone] = '\0';
        return nDone;
    }

    // There's a chance we might hit the end of the buffer. Do it the hard way.
    size_t nNeededAfter = 0;
    bool bNearEnd = false;
    LBUF_OFFSET nChar = 0;
    while (iPos < iEnd)
    {
        if (csPrev != m_pcs[iPos.m_point])
        {
            pTransition = ColorTransitionBinary( csPrev, m_pcs[iPos.m_point],
                                                 &nTransition);
        }
        else
        {
            nTransition = 0;
        }
        nChar = utf8_FirstByte[m_autf[iPos.m_byte]];
        if (nBytesMax < nDone + nTransition + nChar + COLOR_MAXIMUM_BINARY_NORMAL)
        {
            if (  !bNearEnd
               || nTransition)
            {
                ColorBinaryNormal(m_pcs[iPos.m_point], &nNeededAfter);
                bNearEnd = true;
            }
            if (nBytesMax < nDone + nTransition + nChar + nNeededAfter)
            {
                // There isn't enough room to add the color sequence,
                // its character, and still get back to normal. Stop here.
                //
                break;
            }
        }
        if (nTransition)
        {
            memcpy(pBuffer + nDone, pTransition, nTransition * sizeof(pTransition[0]));
            nDone += nTransition;
            csPrev = m_pcs[iPos.m_point];
        }
        memcpy(pBuffer + nDone, m_autf + iPos.m_byte, nChar * sizeof(m_autf[0]));
        nDone += nChar;
        cursor_next(iPos);
    }
    pTransition = ColorBinaryNormal(csPrev, &nTransition);
    if (  nTransition
       && nDone + nTransition <= nBytesMax)
    {
        memcpy(pBuffer + nDone, pTransition, nTransition * sizeof(pTransition[0]));
        nDone += nTransition;
    }
    pBuffer[nDone] = '\0';
    return nDone;
}

/*! \brief Generates ANSI string from internal form.
 *
 * \param bColor   Whether or not to output color. Defaults to true.
 * \param bNoBleed Which output mode to use: normal or nobleed.
 *                 Defaults to false (normal).
 * \return         Static buffer.
 */

UTF8 *mux_string::export_TextConverted
(
    bool bColor,
    bool bNoBleed
) const
{
    static UTF8 Buffer[2*LBUF_SIZE];
    if (  0 == m_ncs
       || !bColor)
    {
        export_TextPlain(Buffer);
        return Buffer;
    }

    static const UTF8 *pNormal= T(ANSI_NORMAL ANSI_WHITE);
    size_t nNormal = sizeof(ANSI_NORMAL)-1;
    if (bNoBleed)
    {
        nNormal += sizeof(ANSI_WHITE)-1;
    }

    mux_cursor curIn, iCopy;
    size_t iOut = 0, nCopy = 0, nTransition = 0;
    ColorState csPrev = CS_NORMAL, csCurrent = CS_NORMAL;
    UTF8 *pTransition = NULL;

    bool bPlentyOfRoom = (m_iLast.m_byte + (COLOR_MAXIMUM_ANSI_TRANSITION_LENGTH * m_ncs) + nNormal + 1) < sizeof(Buffer);

    if (bPlentyOfRoom)
    {
        while (curIn < m_iLast)
        {
            csCurrent = m_pcs[curIn.m_point];
            ValidateColorState(csCurrent);
            if (csPrev != csCurrent)
            {
                if (iCopy < curIn)
                {
                    nCopy = (curIn - iCopy).m_byte;
                    memcpy(Buffer + iOut, m_autf + iCopy.m_byte, nCopy);
                    iOut += nCopy;
                    iCopy = curIn;
                }
                pTransition = ColorTransitionANSI( csPrev, csCurrent,
                                                   &nTransition, bNoBleed);
                if (nTransition)
                {
                    memcpy(Buffer + iOut, pTransition, nTransition);
                    iOut += nTransition;
                }
                csPrev = csCurrent;
            }
            cursor_next(curIn);
        }

        if (iCopy < curIn)
        {
            nCopy = (curIn - iCopy).m_byte;
            memcpy(Buffer + iOut, m_autf + iCopy.m_byte, nCopy);
            iOut += nCopy;
        }
        if (csPrev != CS_NORMAL)
        {
            memcpy(Buffer + iOut, pNormal, nNormal);
            iOut += nNormal;
        }
        Buffer[iOut] = '\0';
        return Buffer;
    }

    // There's a chance we might hit the end of the buffer. Do it the hard way.
    size_t nNeededAfter = 0;
    bool bNearEnd = false;
    LBUF_OFFSET nChar = 0;
    while (curIn < m_iLast)
    {
        csCurrent = m_pcs[curIn.m_point];
        ValidateColorState(csCurrent);
        if (csPrev != csCurrent)
        {
            pTransition = ColorTransitionANSI( csPrev, csCurrent,
                                               &nTransition, bNoBleed);
        }
        else
        {
            nTransition = 0;
        }
        nChar = utf8_FirstByte[m_autf[curIn.m_byte]];
        if (sizeof(Buffer) <= iOut + nTransition + nChar + nNormal)
        {
            if (  !bNearEnd
               || nTransition)
            {
                if (  CS_NORMAL == csCurrent
                   || (  bNoBleed
                      && CS_NOBLEED == csCurrent))
                {
                    nNeededAfter = 0;
                }
                else
                {
                    nNeededAfter = nNormal;
                }
                bNearEnd = true;
            }
            if (sizeof(Buffer) <= iOut + nTransition + nChar + nNeededAfter)
            {
                // There isn't enough room to add the color sequence,
                // its character, and still get back to normal. Stop here.
                //
                break;
            }
        }
        if (nTransition)
        {
            memcpy(Buffer + iOut, pTransition, nTransition);
            iOut += nTransition;
            csPrev = csCurrent;
        }
        memcpy(Buffer + iOut, m_autf + curIn.m_byte, nChar);
        iOut += nChar;
        cursor_next(curIn);
    }

    if (  csPrev != CS_NORMAL
       && iOut + nNormal < sizeof(Buffer))
    {
        memcpy(Buffer + iOut, pNormal, nNormal);
        iOut += nNormal;
    }
    Buffer[iOut] = '\0';
    return Buffer;
}

/*! \brief Outputs ANSI-stripped string from internal form.
 *
 * \param buff     Pointer to beginning of lbuf.
 * \param bufc     Pointer to current position. Defaults to NULL.
 * \param nStart   String position to begin copying from. Defaults to 0.
 * \param nLen     Number of chars to copy. Defaults to LBUF_SIZE.
 * \param nBuffer  Size of buffer we're outputting into.
 *                 Defaults to LBUF_SIZE-1.
 * \return         length of resulting string in bytes.
 */

LBUF_OFFSET mux_string::export_TextPlain
(
    UTF8 *pBuffer,
    mux_cursor iStart,
    mux_cursor iEnd,
    size_t nBytesMax
) const
{
    // Sanity check our arguments and find out how much room we have.
    // We assume we're outputting into an LBUF unless given a smaller nBuffer.
    //
    if (  !pBuffer
       || m_iLast <= iStart
       || iEnd <= iStart
       || 0 == nBytesMax)
    {
        return 0;
    }
    if (m_iLast < iEnd)
    {
        iEnd = m_iLast;
    }
    LBUF_OFFSET nBytes  = iEnd.m_byte - iStart.m_byte;
    if (nBytesMax < nBytes)
    {
        nBytes = static_cast<LBUF_OFFSET>(nBytesMax);
    }

    memcpy(pBuffer, m_autf + iStart.m_byte, nBytes);
    pBuffer[nBytes] = '\0';
    return nBytes;
}

/*! \brief Converts and Imports a dbref.
 *
 * \param num      dbref to convert and import.
 * \return         None.
 */

void mux_string::import(dbref num)
{
    realloc_m_pcs(0);

    m_autf[0] = '#';
    LBUF_OFFSET n = 1;

    // mux_ltoa() sets the '\0'.
    //
    n = static_cast<LBUF_OFFSET>(n + mux_ltoa(num, m_autf + 1));
    m_iLast(n, n);
}

/*! \brief Converts and Imports an INT64.
 *
 * \param iInt     INT64 to convert and import.
 * \return         None.
 */

void mux_string::import(INT64 iInt)
{
    realloc_m_pcs(0);

    // mux_i64toa() sets the '\0'.
    //
    LBUF_OFFSET n = (LBUF_OFFSET)mux_i64toa(iInt, m_autf);
    m_iLast(n, n);
}

/*! \brief Converts and Imports an long integer.
 *
 * \param lLong    long integer to convert and import.
 * \return         None.
 */

void mux_string::import(long lLong)
{
    realloc_m_pcs(0);

    // mux_ltoa() sets the '\0'.
    //
    LBUF_OFFSET n = (LBUF_OFFSET)mux_ltoa(lLong, m_autf);
    m_iLast(n, n);
}

/*! \brief Import a portion of another mux_string.
 *
 * \param sStr     mux_string to import.
 * \param iStart   Where to begin importing.
 * \return         None.
 */

void mux_string::import(const mux_string &sStr, mux_cursor iStart)
{
    if (sStr.m_iLast <= iStart)
    {
        m_iLast = CursorMin;
        m_autf[m_iLast.m_byte] = '\0';
        realloc_m_pcs(0);
    }
    else
    {
        m_iLast = sStr.m_iLast - iStart;
        memcpy(m_autf, sStr.m_autf + iStart.m_byte, m_iLast.m_byte);
        m_autf[m_iLast.m_byte] = '\0';

        if (0 == sStr.m_ncs)
        {
            realloc_m_pcs(0);
        }
        else
        {
            realloc_m_pcs(m_iLast.m_point);
            memcpy(m_pcs, sStr.m_pcs + iStart.m_point, m_iLast.m_point * sizeof(m_pcs[0]));
        }
    }
}

/*! \brief Import ANSI string.
 *
 * Parses the given ANSI string into a form which can be more-easily
 * navigated.
 *
 * \param pStr     ANSI-color encoded string to import.
 * \return         None.
 */

void mux_string::import(const UTF8 *pStr)
{
    m_iLast = CursorMin;
    if (  NULL == pStr
       || '\0' == *pStr)
    {
        m_autf[m_iLast.m_byte] = '\0';
        return;
    }

    size_t nLen = strlen((char *)pStr);
    import(pStr, nLen);
}

/*! \brief Import ANSI string.
 *
 * Parses the given ANSI string into a form which can be more-easily
 * navigated.
 *
 * \param pStr     ANSI-color encoded string to import.
 * \param nLen     Length of portion of string, str, to import.
 * \return         None.
 */

void mux_string::import(const UTF8 *pStr, size_t nLen)
{
    m_iLast = CursorMin;

    if (  NULL == pStr
       || '\0' == *pStr
       || 0 == nLen)
    {
        realloc_m_pcs(0);
        m_autf[m_iLast.m_byte] = '\0';
        return;
    }

    if (LBUF_SIZE-1 < nLen)
    {
        nLen = LBUF_SIZE-1;
    }

    bool bColor = false;
    static ColorState acsTemp[LBUF_SIZE];
    ColorState cs = CS_NORMAL;

    LBUF_OFFSET iPoint = 0;
    size_t iStr = 0;
    UTF8 *pch = m_autf;
    while (iStr < nLen)
    {
        unsigned int iCode = mux_color(pStr + iStr);
        if (COLOR_NOTCOLOR == iCode)
        {
            safe_chr_utf8(pStr + iStr, m_autf, &pch);
            acsTemp[iPoint++] = cs;
            if (CS_NORMAL != cs)
            {
                bColor = true;
            }
        }
        else
        {
            cs = UpdateColorState(cs, iCode);
        }
        iStr += utf8_FirstByte[(unsigned char)pStr[iStr]];
    }

    m_iLast(static_cast<LBUF_OFFSET>(pch - m_autf), iPoint);
    m_autf[m_iLast.m_byte] = '\0';

    if (bColor)
    {
        realloc_m_pcs(m_iLast.m_point);
        memcpy(m_pcs, acsTemp, m_iLast.m_point * sizeof(m_pcs[0]));
    }
    else
    {
        realloc_m_pcs(0);
    }
}

void mux_string::prepend(dbref num)
{
    mux_string *sStore = new mux_string(*this);

    import(num);
    append(*sStore);
    delete sStore;
}

void mux_string::prepend(long lLong)
{
    mux_string *sStore = new mux_string(*this);

    import(lLong);
    append(*sStore);
    delete sStore;
}

void mux_string::prepend(INT64 iInt)
{
    mux_string *sStore = new mux_string(*this);

    import(iInt);
    append(*sStore);
    delete sStore;
}

void mux_string::prepend(const mux_string &sStr)
{
    mux_string *sStore = new mux_string(*this);

    import(sStr);
    append(*sStore);
    delete sStore;
}

void mux_string::prepend(const UTF8 *pStr)
{
    mux_string *sStore = new mux_string(*this);

    import(pStr);
    append(*sStore);
    delete sStore;
}

void mux_string::prepend(const UTF8 *pStr, size_t n)
{
    mux_string *sStore = new mux_string(*this);

    import(pStr, n);
    append(*sStore);
    delete sStore;
}

/*! \brief Resizes or deletes the m_pcs array if necessary.
 *
 * If asked to resize the array to 0, this method will delete the
 * array and set m_pcs to NULL.  Otherwise when this method returns
 * the m_pcs array will exist and have at least the required size.
 * Any color states that were already initialized and would fit within
 * the resulting array will be preserved.
 *
 * \param ncs      Size of m_pcs array required.
 * \return         None.
 */

void mux_string::realloc_m_pcs(size_t ncs)
{
    if (  0 == ncs
       && 0 != m_ncs)
    {
        delete [] m_pcs;
        m_pcs = NULL;
        m_ncs = 0;
    }
    else if (m_ncs < ncs)
    {
        // extend in chunks of 8
        //
        ncs |= 0x7;
        ncs++;

        ColorState *pcsOld = m_pcs;
        m_pcs = NULL;
        try
        {
            m_pcs = new ColorState[ncs];
        }
        catch (...)
        {
            ; // Nothing.
        }
        ISOUTOFMEMORY(m_pcs);

        if (0 == m_ncs)
        {
            // Initialize the implicit CS_NORMAL ColorStates to maintain the
            // mux_string invariant.
            //
            int i;
            for (i = 0; i < m_iLast.m_point; i++)
            {
                m_pcs[i] = CS_NORMAL;
            }
        }
        else
        {
            memcpy(m_pcs, pcsOld, m_ncs * sizeof(m_pcs[0]));
            delete [] pcsOld;
        }

        m_ncs = ncs;
    }
}

/*! \brief Replaces a substring within a string with another.
 *
 * \param sTo      Replacement string.
 * \param iStart   Cursor to start of substring to replace.
 * \param nLen     Length of substring to replace.
 */

void mux_string::replace_Chars
(
    const mux_string &sTo,
    mux_cursor iStart,
    mux_cursor nLen
)
{
    mux_cursor nTo = sTo.m_iLast;
    mux_cursor nMove = CursorMin;
    mux_cursor nCopy = nTo;

    if (nLen != nTo)
    {
        // Since the substring size is not the same size as the replacement
        // string size, we need to move things around.
        //
        nMove = m_iLast - (iStart + nLen);
        if (LBUF_SIZE - 1 < m_iLast.m_byte + sTo.m_iLast.m_byte - nLen.m_byte)
        {
            // The resulting string would be too large, so we need to trim
            // either the move or the copy part -- depending on how much
            // needs to be trimmed.
            //
            if (LBUF_SIZE - 1 < iStart.m_byte + nTo.m_byte)
            {
                while (  sTo.cursor_prev(nCopy)
                      && CursorMax.m_byte - iStart.m_byte < nCopy.m_byte)
                {
                    ; // Nothing.
                }
                nMove = CursorMin;
            }
            else
            {
                while (  cursor_prev(nMove)
                      && CursorMax.m_byte - (iStart.m_byte + nCopy.m_byte) < nMove.m_byte)
                {
                    ; // Nothing.
                }
            }
        }

        mux_cursor iLast = iStart + nCopy + nMove;

        // Move text if necessary.
        //
        if (CursorMin < nMove)
        {
            memmove(m_autf + iStart.m_byte + nCopy.m_byte,
                    m_autf + iStart.m_byte + nLen.m_byte, nMove.m_byte * sizeof(m_autf[0]));
        }

        // Update color if necessary to be consistent with the move.
        //
        if (0 != m_ncs)
        {
            realloc_m_pcs(iLast.m_point);

            // Propagate the last color state out further.
            //
            for (int i = m_iLast.m_point; i < iLast.m_point; i++)
            {
                m_pcs[i] = m_pcs[m_iLast.m_point-1];
            }

            // If there is a move operation, the color also needs to be moved.
            // A copy on the end of the string without a move may require some
            // color state to be initialized.
            //
            if (CursorMin < nMove)
            {
                memmove(m_pcs + iStart.m_point + nCopy.m_point,
                        m_pcs + iStart.m_point + nLen.m_point, nMove.m_point * sizeof(m_pcs[0]));
            }
        }
        else if (0 != sTo.m_ncs)
        {
            realloc_m_pcs(iLast.m_point);

            // Propagate CS_NORMAL out further.
            //
            for (int i = iStart.m_point + nCopy.m_point; i < iLast.m_point; i++)
            {
                m_pcs[i] = CS_NORMAL;
            }
        }

        m_iLast = iLast;
    }
    else if (  0 == m_ncs
            && 0 != sTo.m_ncs)
    {
        realloc_m_pcs(m_iLast.m_point);
    }

    // Copy replacement text over substring.
    //
    memcpy(m_autf + iStart.m_byte, sTo.m_autf, nCopy.m_byte * sizeof(m_autf[0]));

    // If the replacement string contains color, replace the color as well.
    //
    if (0 != sTo.m_ncs)
    {
        memcpy(m_pcs + iStart.m_point, sTo.m_pcs, nCopy.m_point * sizeof(m_pcs[0]));
    }

    m_autf[m_iLast.m_byte] = '\0';
}

bool mux_string::replace_Point(const UTF8 *p, const mux_cursor &i)
{
    size_t n = utf8_FirstByte[m_autf[i.m_byte]];
    size_t m = utf8_FirstByte[p[0]];

    if (n != m)
    {
        if (  m < n
           && sizeof(m_autf) <= m_iLast.m_byte + m - n)
        {
            // We need to truncate the trailing point to make room for an expansion.
            //
            do
            {
                cursor_prev(m_iLast);
            } while (sizeof(m_autf) <= m_iLast.m_byte + m - n);
            m_autf[m_iLast.m_byte] = '\0';
        }

        if (i.m_byte + n  < m_iLast.m_byte)
        {
            size_t nBytesMove = m_iLast.m_byte - (i.m_byte + n);
            UTF8 *pFrom = m_autf + i.m_byte + n;
            UTF8 *pTo   = m_autf + i.m_byte + m;
            memmove(pTo, pFrom, nBytesMove);

            m_iLast.m_byte += m - n;
            m_autf[m_iLast.m_byte] = '\0';
        }
    }

    for (size_t j = 0; j < m; j++)
    {
        m_autf[i.m_byte + j] = p[j];
    }
    return true;
}

void mux_string::replace_Char(const mux_cursor &i, const mux_string &sStr, const mux_cursor &j)
{
    this->replace_Point(sStr.m_autf+j.m_byte, i);
}

/*! \brief Reverses the string.
 *
 * \return         None.
 */

void mux_string::reverse(void)
{
    mux_string *sTemp = new mux_string;
    sTemp->realloc_m_pcs(m_ncs);

    mux_cursor i = m_iLast, j = i;

    while (cursor_prev(i))
    {
        sTemp->append(*this, i, j);
        j = i;
    }
    import(*sTemp);
    delete sTemp;
}

/*! \brief Searches text for a specified pattern.
 *
 * \param pPattern Pointer to pattern to search for.
 * \param nPos     Pointer to value of position in string where pattern
                   is found.
 * \param nStart   Position in string to begin looking at. Defaults to 0.
 * \return         True if found, false if not.
 */

bool mux_string::search
(
    const UTF8 *pPattern,
    mux_cursor *iPos,
    mux_cursor iStart,
    mux_cursor iEnd
) const
{
    // Strip ANSI from pattern.
    //
    size_t nPat = 0;
    UTF8 *pPatBuf = strip_color(pPattern, &nPat);
    const UTF8 *pTarget = m_autf + iStart.m_byte;

    if (m_iLast < iEnd)
    {
        iEnd = m_iLast;
    }

    size_t i = 0;
    bool bSucceeded = false;
    if (nPat == 1)
    {
        // We can optimize the single-character case.
        //
        const unsigned char *p = (const unsigned char *)memchr(pTarget, pPatBuf[0], iEnd.m_byte - iStart.m_byte);
        if (p)
        {
            i = p - pTarget;
            bSucceeded = true;
        }
    }
    else if (nPat > 1)
    {
        // We have a multi-byte pattern.
        //
        bSucceeded = BMH_StringSearch(&i, nPat, pPatBuf,
                                      iEnd.m_byte - iStart.m_byte, pTarget);
    }

    if (iPos)
    {
        cursor_from_byte(*iPos, static_cast<LBUF_OFFSET>(i + iStart.m_byte));
    }
    return bSucceeded;
}

/*! \brief Searches text for a specified pattern.
 *
 * \param sPattern Reference to string to search for.
 * \param nPos     Pointer to value of position in string where pattern
                   is found.
 * \param nStart   Position in string to begin looking at. Defaults to 0.
 * \return         True if found, false if not.
 */

bool mux_string::search
(
    const mux_string &sPattern,
    mux_cursor *iPos,
    mux_cursor iStart,
    mux_cursor iEnd
) const
{
    // Strip ANSI from pattern.
    //
    const UTF8 *pTarget = m_autf + iStart.m_byte;

    size_t i = 0;
    bool bSucceeded = false;
    if (m_iLast < iEnd)
    {
        iEnd = m_iLast;
    }
    if (1 == sPattern.m_iLast.m_byte)
    {
        // We can optimize the single-character case.
        //
        const unsigned char *p = (const unsigned char *)memchr(pTarget, sPattern.m_autf[0], iEnd.m_byte - iStart.m_byte);
        if (p)
        {
            i = p - pTarget;
            bSucceeded = true;
        }
    }
    else
    {
        // We have a multi-byte pattern.
        //
        bSucceeded = BMH_StringSearch(&i, sPattern.m_iLast.m_byte, sPattern.m_autf,
                                      iEnd.m_byte - iStart.m_byte, pTarget);
    }

    if (iPos)
    {
        cursor_from_byte(*iPos, static_cast<LBUF_OFFSET>(i + iStart.m_byte));
    }
    return bSucceeded;
}

// This function is deprecated.
//
void mux_string::set_Char(size_t n, const UTF8 cChar)
{
    if (m_iLast.m_byte <= n)
    {
        return;
    }
    m_autf[n] = cChar;
}

void mux_string::set_Color(size_t n, ColorState csColor)
{
    ValidateColorState(csColor);

    if (m_iLast.m_point <= n)
    {
        return;
    }
    if (  0 == m_ncs
       && csColor != CS_NORMAL)
    {
        realloc_m_pcs(m_iLast.m_point);
    }

    if (0 != m_ncs)
    {
        m_pcs[n] = csColor;
    }
}

bool mux_string::compare_Char(const mux_cursor &i, const mux_string &sStr) const
{
    return (  sStr.m_iLast.m_byte <= m_iLast.m_byte - i.m_byte
           && 0 == memcmp(m_autf + i.m_byte, sStr.m_autf, sStr.m_iLast.m_byte));
}

/*! \brief Removes a specified set of characters from string.
 *
 * \param pStripSet Pointer to string of characters to remove.
 * \param nStart    Position in string to begin checking. Defaults to 0.
 * \param nLen      Number of characters in string to check.
                    Defaults to LBUF_SIZE-1.
 * \return          None.
 */

void mux_string::strip(const UTF8 *pStripSet, mux_cursor iStart, mux_cursor iEnd)
{
    static bool strip_table[UCHAR_MAX+1];

    if (  NULL == pStripSet
       || '\0' == pStripSet[0]
       || m_iLast <= iStart
       || iEnd <= iStart)
    {
        // Nothing to do.
        //
        return;
    }

    if (m_iLast < iEnd)
    {
        iEnd = m_iLast;
    }

    // Load set of characters to strip.
    //
    memset(strip_table, false, sizeof(strip_table));
    while ('\0' != *pStripSet)
    {
        UTF8 ch = *pStripSet++;
        if (mux_isprint_ascii(ch))
        {
            strip_table[ch] = true;
        }
    }
    stripWithTable(strip_table, iStart, iEnd);
}

void mux_string::stripWithTable
(
    const bool strip_table[UCHAR_MAX+1],
    mux_cursor iStart,
    mux_cursor iEnd
)
{
    if (  m_iLast <= iStart
       || iEnd <= iStart)
    {
        // Nothing to do.
        //
        return;
    }

    if (m_iLast < iEnd)
    {
        iEnd = m_iLast;
    }

    bool bInStrip = false;
    mux_cursor iStripStart = iStart;
    for (mux_cursor i = iStart; i < iEnd; cursor_next(i))
    {
        if (  !bInStrip
           && strip_table[m_autf[i.m_byte]])
        {
            bInStrip = true;
            iStripStart = i;
        }
        else if (  bInStrip
                && !strip_table[m_autf[i.m_byte]])
        {
            // We've hit the end of a string to be stripped.
            //
            delete_Chars(iStripStart, i);
            iEnd -= (i - iStripStart);
            i = iStripStart;
            bInStrip = false;
        }
    }

    if (bInStrip)
    {
        if (m_iLast == iEnd)
        {
            // We found chars to strip at the end of the string.
            // We can just truncate.
            //
            m_iLast = iStripStart;
            m_autf[m_iLast.m_byte] = '\0';
        }
        else
        {
            delete_Chars(iStripStart, iEnd);
        }
    }
}

void mux_string::transform
(
    mux_string &sFromSet,
    mux_string &sToSet,
    size_t nStart,
    size_t nLen
)
{
    if (m_iLast.m_point <= nStart)
    {
        return;
    }

    if (  sFromSet.isAscii()
       && sToSet.isAscii())
    {
        // Since both sets use only ASCII characters, we can special case this
        // request.  First, we build a table that maps all possible ASCII
        // characters to their requested replacement, and then we use it.
        //
        UTF8 asciiTable[SCHAR_MAX+1];
        for (unsigned char c = 0; c <= SCHAR_MAX; c++)
        {
            asciiTable[c] = c;
        }

        mux_cursor iFromSet, iToSet;
        sFromSet.cursor_start(iFromSet);
        sToSet.cursor_start(iToSet);
        do
        {
            UTF8 cFrom = sFromSet.m_autf[iFromSet.m_byte];
            UTF8 cTo = sToSet.m_autf[iToSet.m_byte];
            asciiTable[cFrom] = cTo;
        } while (  sFromSet.cursor_next(iFromSet)
                && sToSet.cursor_next(iToSet));

        transform_Ascii(asciiTable, nStart, nLen);
    }
    else
    {
        // This is the more general case.  We use a hash table for mapping.
        //
        hashreset(&mudstate.scratch_htab);

        mux_cursor iFromSet, iToSet;
        sFromSet.cursor_start(iFromSet);
        sToSet.cursor_start(iToSet);
        do
        {
            size_t nFrom = utf8_FirstByte[sFromSet.m_autf[iFromSet.m_byte]];
            hashdeleteLEN(&sFromSet.m_autf[iFromSet.m_byte], nFrom, &mudstate.scratch_htab);
            hashaddLEN(&sFromSet.m_autf[iFromSet.m_byte], nFrom, &sToSet.m_autf[iToSet.m_byte], &mudstate.scratch_htab);

        } while (  sFromSet.cursor_next(iFromSet)
                && sToSet.cursor_next(iToSet));

        mux_cursor i;
        if (cursor_from_point(i, nStart))
        {
            do
            {
                size_t n = utf8_FirstByte[m_autf[i.m_byte]];
                UTF8 *p = static_cast<UTF8*>(hashfindLEN(&m_autf[i.m_byte], n, &mudstate.scratch_htab));
                if (  NULL != p
                   && !replace_Point(p, i))
                {
                    break;
                }
            } while (  cursor_next(i)
                    && i.m_point < nStart+nLen);
        }

        hashreset(&mudstate.scratch_htab);
    }
}

void mux_string::transform_Ascii
(
    const UTF8 asciiTable[SCHAR_MAX+1],
    size_t nStart,
    size_t nLen
)
{
    mux_cursor i;
    if (cursor_from_point(i, nStart))
    {
        do
        {
            if (mux_isprint_ascii(m_autf[i.m_byte]))
            {
                m_autf[i.m_byte] = asciiTable[m_autf[i.m_byte]];
            }
        } while (  cursor_next(i)
                && i.m_point < nStart+nLen);
    }
}

void mux_string::trim(const UTF8 ch, bool bLeft, bool bRight)
{
    if (  0 == m_iLast.m_byte
       || (  !bLeft
          && !bRight ))
    {
        return;
    }

    if (bRight)
    {
        LBUF_OFFSET iPos = m_iLast.m_byte - 1;
        while (  ch == m_autf[iPos]
              && 0 < iPos)
        {
            iPos--;
        }

        if (iPos < m_iLast.m_byte - 1)
        {
            mux_cursor iEnd;
            cursor_end(iEnd);
            while (  iPos < iEnd.m_byte
                  && cursor_prev(iEnd))
            {
                ; // Nothing.
            }
            cursor_next(iEnd);
            m_iLast = iEnd;
            m_autf[m_iLast.m_byte] = '\0';
        }
    }

    if (bLeft)
    {
        LBUF_OFFSET iPos = 0;
        while (  ch == m_autf[iPos]
              && iPos < m_iLast.m_byte)
        {
            iPos++;
        }

        if (0 < iPos)
        {
            mux_cursor iEnd;
            cursor_from_byte(iEnd, iPos);
            delete_Chars(CursorMin, iEnd);
        }
    }
}

void mux_string::trim(const UTF8 *p, bool bLeft, bool bRight)
{
    if (  0 == m_iLast.m_byte
       || NULL == p
       || '\0' == p[0]
       || (  !bLeft
          && !bRight ))
    {
        return;
    }

    size_t n = strlen((char *)p);

    if (1 == n)
    {
        trim(p[0], bLeft, bRight);
    }
    else
    {
        trim(p, n, bLeft, bRight);
    }
}

void mux_string::trim(const UTF8 *p, size_t n, bool bLeft, bool bRight)
{
    if (  CursorMin == m_iLast
       || NULL == p
       || 0 == n
       || m_iLast.m_byte < n
       || (  !bLeft
          && !bRight ))
    {
        return;
    }

    if (bRight)
    {
        LBUF_OFFSET iPos = m_iLast.m_byte - 1;
        size_t iDist = n - 1;
        while (  p[iDist] == m_autf[iPos]
              && 0 < iPos)
        {
            iPos--;
            iDist = (0 < iDist) ? iDist - 1 : n - 1;
        }

        if (iPos < m_iLast.m_byte - 1)
        {
            mux_cursor iEnd;
            cursor_end(iEnd);
            while (  iPos < iEnd.m_byte
                  && cursor_prev(iEnd))
            {
                ; // Nothing.
            }
            cursor_next(iEnd);
            m_iLast = iEnd;
            m_autf[m_iLast.m_byte] = '\0';
        }
    }

    if (bLeft)
    {
        size_t iPos = 0;
        while (  p[iPos % n] == m_autf[iPos]
              && iPos < m_iLast.m_byte)
        {
            iPos++;
        }

        if (0 < iPos)
        {
            mux_cursor iEnd;
            cursor_start(iEnd);
            while (  iEnd.m_byte < iPos
                  && cursor_next(iEnd))
            {
                ; // Nothing.
            }
            delete_Chars(CursorMin, iEnd);
        }
    }
}

void mux_string::truncate(mux_cursor iEnd)
{
    if (m_iLast <= iEnd)
    {
        return;
    }
    m_iLast = iEnd;
    m_autf[m_iLast.m_byte] = '\0';
}

void mux_string::UpperCase(void)
{
    for (mux_cursor i = CursorMin; i < m_iLast; cursor_next(i))
    {
        UTF8 *p = m_autf + i.m_byte;

        bool bXor;
        const string_desc *qDesc = mux_toupper(p, bXor);
        if (NULL != qDesc)
        {
            size_t m = qDesc->n_bytes;
            if (bXor)
            {
                // TODO: In future, the string may need to be expanded or contracted in terms of points.
                //
                size_t j;
                for (j = 0; j < m; j++)
                {
                    p[j] ^= qDesc->p[j];
                }
            }
            else
            {
                // TODO: The string must be expanded or contracted in terms of points and bytes.
                //
            }
        }
    }
}

void mux_string::LowerCase(void)
{
    for (mux_cursor i = CursorMin; i < m_iLast; cursor_next(i))
    {
        UTF8 *p = m_autf + i.m_byte;

        bool bXor;
        const string_desc *qDesc = mux_tolower(p, bXor);
        if (NULL != qDesc)
        {
            size_t m = qDesc->n_bytes;
            if (bXor)
            {
                // TODO: In future, the string may need to be expanded or contracted in terms of points.
                //
                size_t j;
                for (j = 0; j < m; j++)
                {
                    p[j] ^= qDesc->p[j];
                }
            }
            else
            {
                // TODO: The string must be expanded or contracted in terms of points and bytes.
                //
            }
        }
    }
}

void mux_string::UpperCaseFirst(void)
{
    mux_cursor i = CursorMin;
    if (i < m_iLast)
    {
        UTF8 *p = m_autf + i.m_byte;

        bool bXor;
        const string_desc *qDesc = mux_totitle(p, bXor);
        if (NULL != qDesc)
        {
            size_t m = qDesc->n_bytes;
            if (bXor)
            {
                // TODO: In future, the string may need to be expanded or contracted in terms of points.
                //
                size_t j;
                for (j = 0; j < m; j++)
                {
                    p[j] ^= qDesc->p[j];
                }
            }
            else
            {
                // TODO: The string must be expanded or contracted in terms of points and bytes.
                //
            }
        }
    }
}

mux_words::mux_words(const mux_string &sStr) : m_s(&sStr)
{
    m_aiWordBegins[0] = CursorMin;
    m_aiWordEnds[0] = CursorMin;
    m_nWords = 0;
}

void mux_words::export_WordColor(LBUF_OFFSET n, UTF8 *buff, UTF8 **bufc)
{
    if (m_nWords <= n)
    {
        return;
    }

    mux_cursor iStart = m_aiWordBegins[n];
    mux_cursor iEnd = m_aiWordEnds[n];
    size_t nMax = buff + (LBUF_SIZE-1) - *bufc;
    *bufc += m_s->export_TextColor(*bufc, iStart, iEnd, nMax);
}

LBUF_OFFSET mux_words::find_Words(void)
{
    mux_cursor n = m_s->m_iLast;
    LBUF_OFFSET nWords = 0;
    bool bPrev = true;

    for (mux_cursor i = CursorMin; i < n; m_s->cursor_next(i))
    {
        if (  !bPrev
           && m_aControl[m_s->m_autf[i.m_byte]])
        {
            bPrev = true;
            m_aiWordEnds[nWords] = i;
            nWords++;
        }
        else if (bPrev)
        {
            bPrev = false;
            m_aiWordBegins[nWords] = i;
        }
    }
    if (!bPrev)
    {
        m_aiWordEnds[nWords] = n;
        nWords++;
    }
    m_nWords = nWords;
    return m_nWords;
}

LBUF_OFFSET mux_words::find_Words(const UTF8 *pDelim)
{
    pDelim = strip_color(pDelim);
    mux_cursor nDelim = CursorMin;
    utf8_strlen(pDelim, nDelim);

    mux_cursor iPos = CursorMin;
    mux_cursor iStart = CursorMin;
    LBUF_OFFSET nWords = 0;
    bool bSucceeded = m_s->search(pDelim, &iPos, iStart);

    while (  bSucceeded
          && nWords + 1 < MAX_WORDS)
    {
        m_aiWordBegins[nWords] = iStart;
        m_aiWordEnds[nWords] = iPos;
        nWords++;
        iStart = iPos + nDelim;
        bSucceeded = m_s->search(pDelim, &iPos, iStart);
    }
    m_aiWordBegins[nWords] = iStart;
    m_aiWordEnds[nWords] = m_s->m_iLast;
    nWords++;
    m_nWords = nWords;
    return nWords;
}

void mux_words::ignore_Word(LBUF_OFFSET n)
{
    if (m_nWords <= n)
    {
        return;
    }

    for (LBUF_OFFSET i = n; i < m_nWords - 1; i++)
    {
        m_aiWordBegins[i] = m_aiWordBegins[i + 1];
        m_aiWordEnds[i] = m_aiWordEnds[i + 1];
    }
    m_nWords--;
}

void mux_words::set_Control(const UTF8 *pControlSet)
{
    if (  NULL == pControlSet
       || '\0' == pControlSet[0])
    {
        // Nothing to do.
        //
        return;
    }

    // Load set of characters.
    //
    memset(m_aControl, false, sizeof(m_aControl));
    while (*pControlSet)
    {
        UTF8 ch = *pControlSet++;
        if (mux_isprint_ascii(ch))
        {
            m_aControl[ch] = true;
        }
    }
}

void mux_words::set_Control(const bool table[UCHAR_MAX+1])
{
    memcpy(m_aControl, table, sizeof(table));
}

mux_cursor mux_words::wordBegin(LBUF_OFFSET n) const
{
    if (m_nWords <= n)
    {
        return CursorMin;
    }

    return m_aiWordBegins[n];
}

mux_cursor mux_words::wordEnd(LBUF_OFFSET n) const
{
    if (m_nWords <= n)
    {
        return CursorMin;
    }

    return m_aiWordEnds[n];
}
