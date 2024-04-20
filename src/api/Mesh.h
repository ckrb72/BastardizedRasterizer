#pragma once

#include <iostream>
#include "../math/lnal.h"

class Mesh
{
private:
    unsigned int vertex_array;
    unsigned int vertex_buffer;
    unsigned int index_buffer;

    uint32_t m_vertex_count;

    lnal::mat4 model;

public:
    Mesh();

    ~Mesh();

    bool load_file(const std::string& filepath);

    
};