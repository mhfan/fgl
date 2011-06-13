#include "fgl_sphere.h"

#include <libfixmath/fixmath.h>

#include "fgl_transform.h"
#include "fgl_matrix.h"
#include "fgl_vertex.h"
#include "fgl_color.h"
#include "fgl_draw.h"

#include "fgl_texture.h"
extern fgl_texture* _fgl_texture;


void fgl_draw_sphere(uint32_t inDetail) {
	if(inDetail < 3)
		inDetail = 3;

	fgl_vertex_t tempVerts[inDetail << 1];

	fgl_matrix_push();
	fgl_scale((fix16_one >> 1), (fix16_one >> 1), (fix16_one >> 1));

	uint32_t i, j;

	fix16_t tempSin[inDetail + 1];
	fix16_t tempCos[inDetail + 1];
	fix16_t tempFloat = (fix16_pi << 1) / inDetail;
	for(i = 0; i < inDetail; i++) {
		tempSin[i] = fix16_sin(i * tempFloat);
		tempCos[i] = fix16_cos(i * tempFloat);
	}
	tempSin[inDetail] = tempSin[0];
	tempCos[inDetail] = tempCos[0];

	fix16_t tempLatitudes[inDetail];
	for(i = 0; i < inDetail; i++) {
		tempFloat = fix16_div(fix16_from_int(i), fix16_from_int(inDetail));
		tempFloat = tempFloat - (fix16_one >> 1);
		tempFloat = fix16_mul(tempFloat, -fix16_pi);
		tempFloat = fix16_sin(tempFloat);
		tempLatitudes[i] = tempFloat;
	}

	fix16_t tempScales[inDetail];
	for(i = 0; i < inDetail; i++) {
		tempFloat = fix16_div(fix16_from_int(i), fix16_from_int(inDetail));
		tempFloat = tempFloat - (fix16_one >> 1);
		tempFloat = fix16_mul(tempFloat, -fix16_pi);
		tempFloat = fix16_cos(tempFloat);
		tempScales[i] = tempFloat;
	}

	fix16_t tempU[2];
	fix16_t tempV;

	uint32_t tempIndex;
	for(i = 0; i < inDetail; i++) {
		tempU[0] = fix16_one - fix16_div(fix16_from_int(i), fix16_from_int(inDetail));
		tempU[1] = fix16_one - fix16_div(fix16_from_int(i + 1), fix16_from_int(inDetail));

		tempU[0] >>= (16 - _fgl_texture->width);
		tempU[1] >>= (16 - _fgl_texture->width);

		tempVerts[0].x = 0;
		tempVerts[0].z = 0;
		tempVerts[0].y = fix16_one;
		tempVerts[0].u = (tempU[0] + tempU[1]) >> 1;
		tempVerts[0].v = 0;
		tempVerts[0].c = FGL_COLOR_WHITE;

		for(j = 1, tempIndex = 1; j < inDetail; j++, tempIndex += 2) {
			tempV = fix16_div(fix16_from_int(j), fix16_from_int(inDetail));
			tempV >>= (16 - _fgl_texture->height);

			tempVerts[tempIndex + 0].x = fix16_mul(tempSin[i], tempScales[j]);
			tempVerts[tempIndex + 0].z = fix16_mul(tempCos[i], tempScales[j]);
			tempVerts[tempIndex + 0].y = tempLatitudes[j];
			tempVerts[tempIndex + 0].u = tempU[0];
			tempVerts[tempIndex + 0].v = tempV;
			tempVerts[tempIndex + 0].c = FGL_COLOR_WHITE;

			tempVerts[tempIndex + 1].x = fix16_mul(tempSin[i + 1], tempScales[j]);
			tempVerts[tempIndex + 1].z = fix16_mul(tempCos[i + 1], tempScales[j]);
			tempVerts[tempIndex + 1].y = tempLatitudes[j];
			tempVerts[tempIndex + 1].u = tempU[1];
			tempVerts[tempIndex + 1].v = tempV;
			tempVerts[tempIndex + 1].c = FGL_COLOR_WHITE;
		}

		tempVerts[(inDetail << 1) - 1].x = 0;
		tempVerts[(inDetail << 1) - 1].z = 0;
		tempVerts[(inDetail << 1) - 1].y = -fix16_one;
		tempVerts[(inDetail << 1) - 1].u = (tempU[0] + tempU[1]) >> 1;
		tempVerts[(inDetail << 1) - 1].v = (1 << _fgl_texture->height);
		tempVerts[(inDetail << 1) - 1].c = FGL_COLOR_WHITE;

		fgl_draw_array(FGL_TRIANGLE_STRIP, tempVerts, (inDetail << 1));
	}

	fgl_matrix_pop();
}
