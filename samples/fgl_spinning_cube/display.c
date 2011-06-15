//SDL PORT FGL_SPINNING_CUBE

#include "display.h"
/*#include <GL/glew.h>
#include <GL/glfw.h>*/
#include <SDL/SDL.h>
 
SDL_Surface *screen;

#define NULL ( (void*) 0 )

fgl_texture* _display_buffer = NULL;

void display_buffer_set(fgl_texture* inTarget) {
	_display_buffer = inTarget;
}

fgl_texture* display_buffer_get() {
	return _display_buffer;
}



fgl_texture* display_init() {
	/*if(!glfwInit())
		return NULL;

	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	if(!glfwOpenWindow(320, 240, 5, 6, 5, 0, 0, 0, GLFW_WINDOW)) {
		glfwTerminate();
		return NULL;
	}
	glfwSetWindowTitle("display");

	if(glewInit() != GLEW_OK) {
		glfwTerminate();
		return NULL;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int tempViewport[4];
	glGetIntegerv(GL_VIEWPORT, tempViewport);
	glOrtho(0, tempViewport[2], tempViewport[3], 0, -1, 1);
	if(glGetError() != GL_NO_ERROR) {
		//glfwTerminate();
		return false;
	}*/

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
	    return NULL;
	}

	screen = SDL_SetVideoMode(320, 240, 16, SDL_SWSURFACE);
	if (screen == NULL) {
		fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
		return NULL;
	}

	_display_buffer = fgl_texture_create(320, 240);
	if(_display_buffer == NULL) {
		//glfwTerminate();
		return NULL;
	}
	fgl_texture_clear(_display_buffer);
	return _display_buffer;
}

void display_term(){
	//glfwTerminate();
	fgl_texture_delete(_display_buffer);
	_display_buffer = NULL;
}



void display_clear() {
	fgl_texture_clear(_display_buffer);
}

fgl_texture* display_flip() {
	/*glClear(GL_COLOR_BUFFER_BIT);
	uintptr_t i;
	for(i = 0; i < 240; i++) {
		glRasterPos2i(0, i);
		glDrawPixels(320, 1, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, &_display_buffer->data[i * _display_buffer->width]);
	}
	glfwSwapBuffers();
	return _display_buffer;*/
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));	
	memcpy(screen->pixels, _display_buffer->data, screen->pitch*screen->h);
	SDL_Flip(screen);
}

