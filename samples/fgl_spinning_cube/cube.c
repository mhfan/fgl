#include <fgl/fgl.h>

extern fgl_texture* _fgl_texture;

void draw_cube() {

	/*fgl_matrix_mode_set(FGL_MATRIX_MODEL);
	fgl_matrix_identity();	
	fgl_translate(fix16_from_int(x), fix16_from_int(y), fix16_from_int(z));*/

	fgl_vertex_t tempVerts[8];

	uintptr_t i;
	for(i = 0; i < 8; i++) {
		tempVerts[i].x = ((i & 1) ? 1 : -1) << 16;
		tempVerts[i].y = ((i & 2) ? 1 : -1) << 16;
		tempVerts[i].z = ((i & 4) ? 1 : -1) << 16;
		tempVerts[i].u = ((i & 1) ? 0 : (_fgl_texture->width  - 1));
		tempVerts[i].v = ((i & 2) ? 0 : (_fgl_texture->height - 1));
		tempVerts[i].c = 0xFFFF;
	}

	fgl_vertex_t tempFace[4];

	// Front face.
	tempFace[0] = tempVerts[1];
	tempFace[1] = tempVerts[0];
	tempFace[2] = tempVerts[3];
	tempFace[3] = tempVerts[2];
	fgl_draw_array(FGL_TRIANGLE_STRIP, tempFace, 4);

	// Back face.
	tempFace[0] = tempVerts[4];
	tempFace[1] = tempVerts[5];
	tempFace[2] = tempVerts[6];
	tempFace[3] = tempVerts[7];
	fgl_draw_array(FGL_TRIANGLE_STRIP, tempFace, 4);

	// Left face.
	tempFace[0] = tempVerts[0];
	tempFace[1] = tempVerts[4];
	tempFace[2] = tempVerts[2];
	tempFace[3] = tempVerts[6];
	fgl_draw_array(FGL_TRIANGLE_STRIP, tempFace, 4);

	// Right face.
	tempFace[0] = tempVerts[5];
	tempFace[1] = tempVerts[1];
	tempFace[2] = tempVerts[7];
	tempFace[3] = tempVerts[3];
	fgl_draw_array(FGL_TRIANGLE_STRIP, tempFace, 4);

	// Top face.
	tempFace[0] = tempVerts[0];
	tempFace[1] = tempVerts[1];
	tempFace[2] = tempVerts[4];
	tempFace[3] = tempVerts[5];
	fgl_draw_array(FGL_TRIANGLE_STRIP, tempFace, 4);

	// Bottom face.
	tempFace[0] = tempVerts[3];
	tempFace[1] = tempVerts[2];
	tempFace[2] = tempVerts[7];
	tempFace[3] = tempVerts[6];
	fgl_draw_array(FGL_TRIANGLE_STRIP, tempFace, 4);
}

void draw_wire_cube() {
	fgl_vertex_t tempVerts[8];

	uintptr_t i;
	for(i = 0; i < 8; i++) {
		tempVerts[i].x = ((i & 1) ? 1 : -1) << 16;
		tempVerts[i].y = ((i & 2) ? 1 : -1) << 16;
		tempVerts[i].z = ((i & 4) ? 1 : -1) << 16;
		tempVerts[i].u = ((i & 1) ? 0 : (_fgl_texture->width  - 1));
		tempVerts[i].v = ((i & 2) ? 0 : (_fgl_texture->height - 1));
		tempVerts[i].c = 0xFFFF;
	}

	fgl_vertex_t tempFace[4];

	// Front face.
	tempFace[0] = tempVerts[1];
	tempFace[1] = tempVerts[0];
	tempFace[2] = tempVerts[3];
	tempFace[3] = tempVerts[2];
	fgl_draw_array(FGL_LINE_LOOP, tempFace, 4);

	// Back face.
	tempFace[0] = tempVerts[4];
	tempFace[1] = tempVerts[5];
	tempFace[2] = tempVerts[6];
	tempFace[3] = tempVerts[7];
	fgl_draw_array(FGL_LINE_LOOP, tempFace, 4);

	// Left face.
	tempFace[0] = tempVerts[0];
	tempFace[1] = tempVerts[4];
	tempFace[2] = tempVerts[2];
	tempFace[3] = tempVerts[6];
	fgl_draw_array(FGL_LINE_LOOP, tempFace, 4);

	// Right face.
	tempFace[0] = tempVerts[5];
	tempFace[1] = tempVerts[1];
	tempFace[2] = tempVerts[7];
	tempFace[3] = tempVerts[3];
	fgl_draw_array(FGL_LINE_LOOP, tempFace, 4);

	// Top face.
	tempFace[0] = tempVerts[0];
	tempFace[1] = tempVerts[1];
	tempFace[2] = tempVerts[4];
	tempFace[3] = tempVerts[5];
	fgl_draw_array(FGL_LINE_LOOP, tempFace, 4);

	// Bottom face.
	tempFace[0] = tempVerts[3];
	tempFace[1] = tempVerts[2];
	tempFace[2] = tempVerts[7];
	tempFace[3] = tempVerts[6];
	fgl_draw_array(FGL_LINE_LOOP, tempFace, 4);
}
