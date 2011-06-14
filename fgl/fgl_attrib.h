#ifndef __fgl_attrib_h__
#define __fgl_attrib_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

static const uint8_t FGL_TEXTURE_2D          = 1
static const uint8_t FGL_DEPTH_TEST          = 2
static const uint8_t FGL_DEPTH_WRITE         = 3
static const uint8_t FGL_TRANSFORM           = 4
static const uint8_t FGL_CLIP                = 5
static const uint8_t FGL_CULL_FACE           = 6 // TODO
static const uint8_t FGL_TEXTURE_PERSPECTIVE = 7 // TODO

extern bool fgl_enable(uint8_t inCap);
extern bool fgl_disable(uint8_t inCap);

#ifdef __cplusplus
}
#endif

#endif
