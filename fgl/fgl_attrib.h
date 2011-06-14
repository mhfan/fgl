#ifndef __fgl_attrib_h__
#define __fgl_attrib_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#define FGL_TEXTURE_2D          1
#define FGL_DEPTH_TEST          2
#define FGL_DEPTH_WRITE         3
#define FGL_TRANSFORM           4
#define FGL_CLIP                5
#define FGL_CULL_FACE           6 // TODO
#define FGL_TEXTURE_PERSPECTIVE 7 // TODO

extern bool fgl_enable(uint8_t inCap);
extern bool fgl_disable(uint8_t inCap);

#ifdef __cplusplus
}
#endif

#endif
