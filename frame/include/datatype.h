#ifndef __DATATYPE_H__
#define __DATATYPE_H__

typedef char            CHAR;
typedef signed char     INT8;
typedef unsigned char   UINT8;
typedef signed short    INT16;
typedef unsigned short  UINT16;
typedef signed int      INT32;
typedef unsigned int    UINT32;
typedef unsigned int      FD_T;

#define LUAMAN_DIM(_a_) (sizeof(_a_)/sizeof((_a_)[0]))

#endif