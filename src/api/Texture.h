#pragma once

#include <iostream>

class Texture
{
private:
    uint32_t m_texture;
    int32_t m_width;
    int32_t m_height;
    int32_t m_channels;

    //Might want to do some clipping thing

    void gen_texture_gpu(const unsigned char* texture, uint32_t width, uint32_t height, uint32_t channels);

public:
    Texture();

    ~Texture();

    bool load_texture(const std::string& filepath);

    void bind(uint32_t unit);

};