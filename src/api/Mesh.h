#pragma once
#include <vector>
#include <string>

#include "../math/lnal.h"

#define MESH_ALL 0xFFFFFFFF
//Don't ever forget the () when defining these.
//Just spent way too much time figuring this out
#define MESH_POSITIONS (1 << 0)
#define MESH_NORMALS (1 << 1)
#define MESH_TEXCOORDS (1 << 2)
#define MESH_COLORS (1 << 3)

class Mesh
{
private:
    uint32_t m_vertex_array;
    uint32_t m_vertex_buffer;
    uint32_t m_index_buffer;

    uint32_t m_vertex_count;

    lnal::mat4 model;

    void gen_buffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, uint32_t flags);

    //Probably want to change this up a bit
    //void set_attribute(uint32_t index, uint32_t size, uint32_t type, bool transpose,  uint32_t stride, void* offset);

public:
    Mesh();

    ~Mesh();

    bool load_model(const std::string& filepath, uint32_t flags);

    
};