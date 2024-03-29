/** -*- c -*-
    Copyright (C) 2001, 2002, 2003  Michael Ahlberg, Måns Rullgård

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

#ifndef	_TCSTRING_H
#define	_TCSTRING_H	1

/* Some systems need this */
#ifndef _REENTRANT
#define _REENTRANT
#endif

#include <stdlib.h>
#include <stdarg.h>


#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <string.h>



#ifndef strdupa
#ifdef __GNUC__
#define strdupa(s)				\
    ({						\
      const char *__old = (s);			\
      size_t __len = strlen(__old) + 1;		\
      char *__new = alloca(__len);		\
      (char *) memcpy(__new, __old, __len);	\
    })
#endif
#endif /* strdupa */

extern int tcstresc(char *dst, const char *src);

extern char *tcstrexp(const char *src, const char *sd, const char *ed,
		      char fs, char *(*lookup)(char *, void *), void *ld,
		      int flags);

#define TCSTREXP_ESCAPE    0x1
#define TCSTREXP_FREE      0x2
#define TCSTREXP_KEEPUNDEF 0x4

extern char *tcregsub(const char *str, const char *pat,
		      const char *sub, int flags);

#endif /* tcstring.h  */
