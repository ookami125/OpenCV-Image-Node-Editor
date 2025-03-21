#pragma once

#include <string.h>

#include <GL/gl.h>

#include "datatypes.h"

struct GLTextureData
{
    GLuint id;
    Image  image;
};

bool LoadTextureFromData(Image image, GLTextureData* tex_data);
bool LoadTextureFromFile(const char* filename, GLTextureData* tex_data);
bool LoadNullTexture(GLTextureData* tex_data);

void RemoveTexture(GLTextureData* tex_data);