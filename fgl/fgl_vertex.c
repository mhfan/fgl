#include "fgl_vertex.h"
#include <stdio.h>

void fgl_vertex_print(fgl_vertex_t inVertex) {
	printf("{ (%+.4f, %+.4f, %+.4f), (%.4f, %.4f), (%.2f, %.2f, %.2f) }\n"
	, fix16_to_dbl(inVertex.x), fix16_to_dbl(inVertex.y), fix16_to_dbl(inVertex.z)
	, fix16_to_dbl(inVertex.u), fix16_to_dbl(inVertex.v)
	, ((double)fgl_color_red(inVertex.c) / 31.0), ((double)fgl_color_green(inVertex.c) / 63.0), ((double)fgl_color_blue(inVertex.c) / 31.0)
	);
}

