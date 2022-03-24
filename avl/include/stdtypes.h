#ifndef __SYS_STDTYPES_H
#define __SYS_STDTYPES_H

typedef enum {
    B_FALSE = 0,
    B_TRUE  = 1,
} boolean_t;

typedef unsigned char      uchar_t;
typedef unsigned short     ushort_t;
typedef unsigned int       uint_t;
typedef unsigned long      ulong_t;
typedef unsigned long long u_longlong_t;
typedef long long          longlong_t;

typedef longlong_t   offset_t;
typedef u_longlong_t u_offset_t;
typedef u_longlong_t len_t;
typedef longlong_t   diskaddr_t;

typedef ulong_t pgcnt_t;   // number of pages
typedef long    spgcnt_t;  // signed number of pages

typedef short    pri_t;
typedef ushort_t o_mode_t;

typedef int major_t;
typedef int minor_t;

typedef short index_t;

#endif  // !__SYS_STDTYPES_H
