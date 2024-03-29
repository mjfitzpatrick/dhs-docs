/**
    Copyright (C) 2001-2004  Michael Ahlberg, Måns Rullgård

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use, copy,
    modify, merge, publish, distribute, sublicense, and/or sell copies
    of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
**/

#ifndef _TCENDIAN_H
#define _TCENDIAN_H

#define TCENDIAN_LITTLE 1
#define TCENDIAN_BIG 2


#include <byteswap.h>

#define TCENDIAN TCENDIAN_LITTLE

#define htol_16(x) (x)
#define htol_32(x) (x)
#define htol_64(x) (x)

#define htob_16(x) (bswap_16(x))
#define htob_32(x) (bswap_32(x))
#define htob_64(x) (bswap_64(x))

#endif
