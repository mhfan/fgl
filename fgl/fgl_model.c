#include "fgl_model.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fgl/fgl_draw.h>
#include <fgl/fgl_color.h>



typedef struct {
    uint32_t ident;              // magic number. must be equal to "IDP2"
    uint32_t version;            // md2 version. must be equal to 8

    uint32_t skinwidth;          // width of the texture
    uint32_t skinheight;         // height of the texture
    uint32_t framesize;          // size of one frame in bytes

    uint32_t num_skins;          // number of textures
    uint32_t num_xyz;            // number of vertices
    uint32_t num_st;             // number of texture coordinates
    uint32_t num_tris;           // number of triangles
    uint32_t num_glcmds;         // number of opengl commands
    uint32_t num_frames;         // total number of frames

    uint32_t offset_skins;       // offset to skin names (64 bytes each)
    uint32_t offset_st;          // offset to s-t texture coordinates
    uint32_t offset_tris;        // offset to triangles
    uint32_t offset_frames;      // offset to frame data
    uint32_t offset_glcmds;      // offset to opengl commands
    uint32_t offset_end;         // offset to end of file
} __attribute__((__packed__)) _md2_header_t;

typedef struct {
	float x, y, z;
} __attribute__((__packed__)) _md2_vector3_t;

typedef struct {
	uint16_t u, v;
} __attribute__((__packed__)) _md2_texcoord2_t;

typedef struct {
	uint8_t x, y, z, n;
} __attribute__((__packed__)) _md2_vertex_t;

typedef struct {
	uint16_t v[3];
	uint16_t t[3];
} __attribute__((__packed__)) _md2_triangle_t;



fgl_model* fgl_model_load_md2(const char* inPath) {
	FILE* tempFile = fopen(inPath, "rb");
	if(tempFile == NULL)
		return NULL;

	_md2_header_t tempHeader;
	if(fread(&tempHeader, sizeof(_md2_header_t), 1, tempFile) < 1) {
		fclose(tempFile);
		return NULL;
	}

	if((strncmp((char*)&tempHeader.ident, "IDP2", 4) != 0) || (tempHeader.version != 8)) {
		fclose(tempFile);
		return NULL;
	}

	fix16_t     tempVerts_u[tempHeader.num_st];
	fix16_t     tempVerts_v[tempHeader.num_st];
	_md2_triangle_t tempTris[tempHeader.num_tris];
	_md2_vertex_t   tempVerts[tempHeader.num_xyz];

	uintptr_t i, j, k;

	fseek(tempFile, tempHeader.offset_st, SEEK_SET);
	for(i = 0; i < tempHeader.num_st; i++) {
		uint16_t tempU, tempV;
		fread(&tempU, 2, 1, tempFile);
		fread(&tempV, 2, 1, tempFile);
		//tempVerts_u[i] = fix16_div(fix16_from_int(tempU), fix16_from_int(tempHeader.skinwidth));
		//tempVerts_v[i] = fix16_div(fix16_from_int(tempV), fix16_from_int(tempHeader.skinheight));
		tempVerts_u[i] = tempU;
		tempVerts_v[i] = tempV;
	}

	fseek(tempFile, tempHeader.offset_tris, SEEK_SET);
	fread(tempTris, sizeof(_md2_triangle_t), tempHeader.num_tris, tempFile);

	uintptr_t  tempModelFrameLen = ((tempHeader.num_tris * 3) * sizeof(fgl_vertex_t));
	fgl_model* tempModel = (fgl_model*)malloc(sizeof(fgl_model) + (tempHeader.num_frames * tempModelFrameLen));
	if(tempModel == NULL) {
		fclose(tempFile);
		return NULL;
	}

	fseek(tempFile, tempHeader.offset_frames, SEEK_SET);

	tempModel->vert_data   = (fgl_vertex_t*)((uintptr_t)tempModel + sizeof(fgl_model));
	tempModel->vert_count  = (tempHeader.num_tris * 3);
	tempModel->frame_count = tempHeader.num_frames;

	fgl_vertex_t* tempPtr = tempModel->vert_data;
	for(i = 0; i < tempModel->frame_count; i++) {
		_md2_vector3_t tempScalef;
		_md2_vector3_t tempTranslatef;
		char tempName[16];

		fread(&tempScalef, sizeof(_md2_vector3_t), 1, tempFile);
		fread(&tempTranslatef, sizeof(_md2_vector3_t), 1, tempFile);
		fread(tempName, 1, 16, tempFile);
		fread(tempVerts, sizeof(_md2_vertex_t), tempHeader.num_xyz, tempFile);

		fix16_t tempScale[3] = {
			fix16_from_float(tempScalef.x),
			fix16_from_float(tempScalef.y),
			fix16_from_float(tempScalef.z)
			};

		fix16_t tempTranslate[3] = {
			fix16_from_float(tempTranslatef.x),
			fix16_from_float(tempTranslatef.y),
			fix16_from_float(tempTranslatef.z)
			};

		for(j = 0; j < tempHeader.num_tris; j++) {
			for(k = 0; k < 3; k++, tempPtr++) {
				tempPtr->x = fix16_from_int(tempVerts[tempTris[j].v[k]].x);
				tempPtr->x = fix16_mul(tempPtr->x, tempScale[0]) + tempTranslate[0];
				tempPtr->y = fix16_from_int(tempVerts[tempTris[j].v[k]].y);
				tempPtr->y = fix16_mul(tempPtr->y, tempScale[1]) + tempTranslate[1];
				tempPtr->z = fix16_from_int(tempVerts[tempTris[j].v[k]].z);
				tempPtr->z = fix16_mul(tempPtr->z, tempScale[2]) + tempTranslate[2];
				tempPtr->u = tempVerts_u[tempTris[j].t[k]];
				tempPtr->v = tempVerts_v[tempTris[j].t[k]];
				tempPtr->c = FGL_COLOR_WHITE;
			}
		}
	}

	fclose(tempFile);
	return tempModel;
}

void fgl_model_delete(fgl_model* inModel) {
	free(inModel);
}



void fgl_model_draw(fgl_model* inModel, fix16_t inFrame) {
	if(inModel == NULL)
		return;

	fgl_vertex_t tempVerts[inModel->vert_count];

	uintptr_t tempFrameNum[2] = { (inFrame >> 16) , ((inFrame + 0xFFFF) >> 16) };

	while(tempFrameNum[0] >= inModel->frame_count) tempFrameNum[0] -= inModel->frame_count;
	while(tempFrameNum[1] >= inModel->frame_count) tempFrameNum[1] -= inModel->frame_count;

	fgl_vertex_t* tempFrame[2];
	tempFrame[0] = &inModel->vert_data[tempFrameNum[0] * inModel->vert_count];
	tempFrame[1] = &inModel->vert_data[tempFrameNum[1] * inModel->vert_count];

	uint16_t tempFract = (inFrame & 0xFFFF);

	uintptr_t i;
	for(i = 0; i < inModel->vert_count; i++) {
		tempVerts[i].x = fix16_lerp16(tempFrame[0][i].x, tempFrame[1][i].x, tempFract);
		tempVerts[i].y = fix16_lerp16(tempFrame[0][i].y, tempFrame[1][i].y, tempFract);
		tempVerts[i].z = fix16_lerp16(tempFrame[0][i].z, tempFrame[1][i].z, tempFract);
		tempVerts[i].u = fix16_lerp16(tempFrame[0][i].u, tempFrame[1][i].u, tempFract);
		tempVerts[i].v = fix16_lerp16(tempFrame[0][i].v, tempFrame[1][i].v, tempFract);
		tempVerts[i].c = FGL_COLOR_WHITE;
	}

	fgl_draw_array(FGL_TRIANGLES, tempVerts, inModel->vert_count);
}
