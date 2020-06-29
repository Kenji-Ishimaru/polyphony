#ifndef __khrplatform_h_
#define __khrplatform_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

typedef signed char khronos_int8_t;
typedef float khronos_float_t;
typedef short khronos_int16_t;
typedef unsigned short khronos_uint16_t;
typedef int khronos_int32_t;
typedef int khronos_ssize_t;
typedef int khronos_intptr_t;
typedef unsigned char khronos_uint8_t;
typedef unsigned long khronos_uint64_t;
typedef long khronos_int64_t;

#define GL_API
#define GL_APIENTRY

#define  GL_GLEXT_PROTOTYPES
#define GL_GLEXT_LEGACY

#ifdef __cplusplus
}
#endif

#endif
