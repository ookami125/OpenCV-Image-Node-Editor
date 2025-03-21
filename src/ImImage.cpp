#include "ImImage.h"

#include <stdexcept>

#include "imgui.h"
#include "imgui_impl_opengl3.h"

#include "stb_image.h"

#include "datatypes.h"

bool LoadTextureFromData(Image image, GLTextureData* tex_data)
{
    tex_data->image = image;
    
    glGenTextures(1, &tex_data->id);
    glBindTexture(GL_TEXTURE_2D, tex_data->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    GLenum format;
    GLenum element_type = GL_UNSIGNED_BYTE;
    int cv_type = tex_data->image.data.type();
    Image mappedImage = tex_data->image;
    switch(cv_type) {
        case CV_8UC3: format = GL_RGB; break;
        case CV_8UC4: format = GL_RGBA; break;
        case CV_32FC3:
            format = GL_RGB;
            tex_data->image.data.convertTo(mappedImage.data, CV_8UC3, 255, 0);
            break;
        case CV_32FC4:
            format = GL_RGBA;
            tex_data->image.data.convertTo(mappedImage.data, CV_8UC4, 255, 0);
            break;
        default: throw("Failed to find texture data type");
    }
    printf("format: %d\n", mappedImage.data.channels());

    int width = mappedImage.width();
    int height = mappedImage.height();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, element_type, mappedImage.data.data);

    return true;
}

bool LoadTextureFromFile(const char* filename, GLTextureData* tex_data)
{
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    Image image(data, width, height, CV_8UC(channels));

    bool ret = LoadTextureFromData(image, tex_data);

    stbi_image_free(data);

    return ret;
}

bool LoadNullTexture(GLTextureData* tex_data)
{
    int width = 2;
    int height = 2;
    unsigned char image_data[16] = {
        255, 0, 255, 255, 0, 0, 0, 255,
        0, 0, 0, 255, 255, 0, 255, 255,
    };

    Image image(image_data, width, height, CV_8UC4);
    return LoadTextureFromData(image, tex_data);
}

void RemoveTexture(GLTextureData* tex_data)
{
    tex_data->id = 0;
}