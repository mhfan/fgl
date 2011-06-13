#ifndef __display_h__
#define __display_h__

#include <stdbool.h>
#include <stdint.h>

#include <fgl/fgl_texture.h>

extern void         display_buffer_set(fgl_texture* inTarget);
extern fgl_texture* display_buffer_get();

extern fgl_texture* display_init();
extern void         display_term();

extern void         display_clear();
extern fgl_texture* display_flip();

#endif
