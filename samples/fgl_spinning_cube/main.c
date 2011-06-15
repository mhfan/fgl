#include <stdlib.h>
#include <stdio.h>

#include <fgl/fgl.h>

#include "display.h"
#include "cube.h"

#include <SDL/SDL.h>

int appRunning = 1;
int startTicks = 0;

float getFps() {
	int tempTicks = SDL_GetTicks();
	float tempFps = 1000.f/(tempTicks - startTicks);
	startTicks = tempTicks;
	return tempFps;
}

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

	/*fgl_texture* tempTexture = fgl_texture_load_tga("splash.tga");
	if (tempTexture == NULL)
		printf("Something went wrong, temptex not loaded.\n");*/
	SDL_Surface *temp = SDL_LoadBMP("cube.bmp");

	if (temp == NULL)
		printf("Something is just not right, where is cube.bmp?\n");

	fgl_texture* tempTexture = fgl_texture_create(temp->w, temp->h);
	memcpy(tempTexture->data, temp->pixels, temp->pitch*temp->h );
	//tempTexture->data = &temp->pixels;
	fgl_texture_bind(tempTexture);
	fgl_enable(FGL_TEXTURE_2D);

	//fgl_disable(FGL_CLIP);

	fgl_enable(FGL_CULL_FACE);

	SDL_Event event;

	while(appRunning) {
		while (SDL_PollEvent(&event))   //Poll our SDL key event for any keystrokes.
		{
			switch(event.type)
			{
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_RETURN:
						appRunning = false;
					break;
					case SDLK_UP:
						fgl_matrix_mode_set(FGL_MATRIX_VIEW);
						fgl_translate(fix16_from_int(0), fix16_from_int(0), fix16_from_int(-10));
						fgl_matrix_mode_set(FGL_MATRIX_MODEL);
					break;
					case SDLK_DOWN:
						fgl_matrix_mode_set(FGL_MATRIX_VIEW);
						fgl_translate(fix16_from_int(0), fix16_from_int(0), fix16_from_int(+10));
						fgl_matrix_mode_set(FGL_MATRIX_MODEL);
					break;
					default:
					break;
				}
				break;
			}
		}
		fgl_clear(FGL_COLOR_BUFFER_BIT | FGL_DEPTH_BUFFER_BIT);
		draw_cube();
		fgl_rotate_y(500);
		fgl_rotate_x(500);
		fgl_rotate_z(500);
		display_flip();
		printf("%f\n", getFps());
	}

	if(tempDepth != NULL)
		free(tempDepth);

	display_term();
	return EXIT_SUCCESS;
}
