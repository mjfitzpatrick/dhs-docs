/** -*- c -*-
    Copyright (C) 2001  Michael Ahlberg, Måns Rullgård

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

#ifndef _TCTYPES_H
#define _TCTYPES_H

#include <sys/types.h>
#include <stdint.h>

/* int8_t is defined */
/* u_int8_t is defined */
/* int16_t is defined */
/* u_int16_t is defined */
/* int32_t is defined */
/* u_int32_t is defined */
/* int64_t is defined */
/* u_int64_t is defined */

#ifdef __GNUC__
#define _unaligned(s)					\
struct _unaligned##s {					\
    uint##s##_t i;					\
} __attribute__((packed));				\
							\
static inline uint##s##_t				\
unaligned##s(const void *v){				\
    return ((const struct _unaligned##s *) v)->i;	\
}							\
							\
static inline void					\
st_unaligned##s(uint##s##_t v, void *d)			\
{							\
    ((struct _unaligned##s *) d)->i = v;		\
}
#else
#include <string.h>
#define _unaligned(s)				\
static inline uint##s##_t			\
unaligned##s(const void *v){			\
    uint##s##_t i;				\
    memcpy(&i, v, sizeof(i));			\
    return i;					\
}						\
						\
static inline void				\
st_unaligned##s(uint##s##_t v, void *d){	\
    memcpy(d, &v, sizeof(v));			\
}
#endif

_unaligned(16)
_unaligned(32)
_unaligned(64)

typedef int (*tccompare_fn) (const void *, const void *);
typedef void (*tcfree_fn) (void *);
typedef size_t (*tcio_fn)(void*, size_t, size_t, void *);


#endif
