#include "Texture.h"

//This should only ever be defined once
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include <glad/glad.h>
#include "../debug.h"


Texture::Texture()
:m_texture(0), m_width(0), m_height(0), m_channels(0)
{}

Texture::~Texture()
{
    glDeleteTextures(1, &m_texture);
}

bool Texture::load_texture(const std::string& filepath)
{
    // Load texture with stb_image

    //If textures come out upside down uncomment this line
    //stbi_set_flip_vertically_on_load(true);

    //Load the image
    unsigned char* image = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channels, 0);

    //If image couldn't be loaded return false
    if(!image)
        return false;


    //Generate the texture on the gpu
    gen_texture_gpu((const unsigned char*)image, m_width, m_height, m_channels);

    //Don't forget to free the image
    stbi_image_free(image);

    return true;
}

//Allocates space and copies the given texture onto the GPU
//If using OpenGL, remember that only square textures are allowed (512x512, 1024x1024, etc.)
//@param texture The texture to put on the GPU
//@param width Width of the Texture
//@param height Height of the Texture
//@param channels Number of Color Channels of the Texture
void Texture::gen_texture_gpu(const unsigned char* texture, uint32_t width, uint32_t height, uint32_t channels)
{

    //Generate the handle for the texture
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Actually generate the texture on the GPU

    //FIXME: NEED TO FIX THIS TO WORK WITH DIFFERENT IMAGE FORMATS
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    gpuCheckError();


    //Generate the mipmaps for the texture
    glGenerateMipmap(GL_TEXTURE_2D);

}

//Bind the texture to the given texture unit
//@param unit The Texture Unit To Bind To
void Texture::bind(uint32_t unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}
