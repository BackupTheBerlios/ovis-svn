#ifndef OVIS_BASE_ION_TYPES_HH_INCLUDED
#define OVIS_BASE_ION_TYPES_HH_INCLUDED

// Visual C types

#ifdef _MSC_VER

typedef signed __int8 ovis_int8;
typedef signed __int16 ovis_int16;
typedef signed __int32 ovis_int32;
typedef signed __int64 ovis_int64;

typedef unsigned __int8 ovis_uint8;
typedef unsigned __int16 ovis_uint16;
typedef unsigned __int32 ovis_uint32;
typedef unsigned __int64 ovis_uint64;

typedef signed __int64 ovis_longlong;
typedef unsigned __int64 ovis_ulonglong;

#define ITYPES_DEFINED	1
#endif

// GNU gcc types

#ifdef __GNUC__

#include <inttypes.h>

typedef int8_t ovis_int8;
typedef int16_t ovis_int16;
typedef int32_t ovis_int32;
typedef int64_t ovis_int64;

typedef uint8_t ovis_uint8;
typedef uint16_t ovis_uint16;
typedef uint32_t ovis_uint32;
typedef uint64_t ovis_uint64;

// TODO: check if the gcc version supports C99 long long's
typedef signed long long ovis_longlong;
typedef unsigned long long ovis_ulonglong;

#define ITYPES_DEFINED	1
#endif

// Unknown platform - break

#ifndef ITYPES_DEFINED
#error Sorry, your platform is unknown. Define the correct types in include/itypes.h
#endif

#endif /*__ION_CORE_INTTYPES_H*/
