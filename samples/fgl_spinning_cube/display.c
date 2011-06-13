#include "display.h"
#include <GL/glew.h>
#include <GL/glfw.h>

fgl_texture* _display_buffer = NULL;

void display_buffer_set(fgl_texture* inTarget) {
	_display_buffer = inTarget;
}

fgl_texture* display_buffer_get() {
	return _display_buffer;
}



fgl_texture* display_init() {
	if(!glfwInit())
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
		glfwTerminate();
		return false;
	}


	_display_buffer = fgl_texture_create(320, 240);
	if(_display_buffer == NULL) {
		glfwTerminate();
		return NULL;
	}
	fgl_texture_clear(_display_buffer);
	return _display_buffer;
}

void display_term(){
	glfwTerminate();
	fgl_texture_delete(_display_buffer);
	_display_buffer = NULL;
}



void display_clear() {
	fgl_texture_clear(_display_buffer);
}

fgl_texture* display_flip() {
	glClear(GL_COLOR_BUFFER_BIT);
	uintptr_t i;
	for(i = 0; i < 240; i++) {
		glRasterPos2i(0, i);
		glDrawPixels(320, 1, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, &_display_buffer->data[i * _display_buffer->width]);
	}
	glfwSwapBuffers();
	return _display_buffer;
}
