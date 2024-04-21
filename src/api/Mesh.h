#pragma once
#include <vector>
#include <string>

#include "../math/lnal.h"


class Mesh
{
private:
    uint32_t m_vertex_array;
    uint32_t m_vertex_buffer;
    uint32_t m_index_buffer;

    uint32_t m_vertex_count;

    lnal::mat4 model;

    void gen_buffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    //Probably want to change this up a bit
    void set_attribute(uint32_t index, uint32_t size, uint32_t type, bool transpose,  uint32_t stride, void* offset);

public:
    Mesh();

    ~Mesh();

    bool load_file(const std::string& filepath);

    
};