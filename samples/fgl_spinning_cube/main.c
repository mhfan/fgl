#include <stdlib.h>
#include <stdio.h>

#include <fgl/fgl.h>

#include "display.h"
#include "cube.h"



int main(int argc, char** argv) {
	if(display_init() == NULL)
		return EXIT_FAILURE;

	fgl_draw_buffer_set(display_buffer_get());
	fgl_texture* tempDrawBuff = display_buffer_get();
	uint16_t* tempDepth = (uint16_t*)malloc((tempDrawBuff->width * tempDrawBuff->height) << 1);
	if(tempDepth != NULL)
		fgl_depth_buffer_set(tempDepth);
	fgl_enable(FGL_DEPTH_TEST);

	fgl_matrix_mode_set(FGL_MATRIX_VIEWPORT);
	fgl_matrix_identity();
	fgl_viewport(0, 0, fix16_from_int(320), fix16_from_int(240));
	fgl_matrix_mode_set(FGL_MATRIX_PROJECTION);
	fgl_matrix_identity();
	fgl_perspective((fix16_pi >> 3), fix16_div(fix16_from_int(320), fix16_from_int(240)), fix16_one, fix16_from_int(1024));
	fgl_ortho_2d(-fix16_one, fix16_one, -fix16_one, fix16_one);
	fgl_matrix_mode_set(FGL_MATRIX_VIEW);
	fgl_matrix_identity();
	fgl_translate(fix16_from_int(0), fix16_from_int(0), fix16_from_int(10));
	fgl_matrix_mode_set(FGL_MATRIX_MODEL);
	fgl_matrix_identity();

	fgl_scale(fix16_from_int(1), fix16_from_int(1), fix16_from_int(1));

	fgl_texture* tempTexture = fgl_texture_load_tga("sunset.tga");
	fgl_texture_bind(tempTexture);
	fgl_enable(FGL_TEXTURE_2D);

	//fgl_disable(FGL_CLIP);

	fgl_enable(FGL_CULL_FACE);

	while(true) {
		fgl_clear(FGL_COLOR_BUFFER_BIT | FGL_DEPTH_BUFFER_BIT);
		draw_cube();
		fgl_rotate_y(500);
		fgl_rotate_x(500);
		fgl_rotate_z(500);
		display_flip();
	}

	if(tempDepth != NULL)
		free(tempDepth);

	display_term();
	return EXIT_SUCCESS;
}
