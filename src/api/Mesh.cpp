#include "Mesh.h"

#include <tiny_obj_loader.h>
#include <glad/glad.h>
#include <vector>

Mesh::Mesh()
:vertex_array(0), vertex_buffer(0), index_buffer(0), model(1.0), m_vertex_count(0)
{}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);
    glDeleteVertexArrays(1, &vertex_array);
}

bool Mesh::load_file(const std::string& filepath)
{
    //First need to check if buffers are already allocated. If so then delete them

    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./";
    
    //Assuming all models are triangulated as of now
    //reader_config.triangulate = true;

    if(!reader.ParseFromFile(filepath, reader_config))
    {
        if(!reader.Error().empty())
        {
            std::cerr << reader.Error();
        }
        return false;
    }

    if(!reader.Warning().empty())
    {
        std::cout << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    std::vector<float> vertices{};

    for(size_t s = 0; s < shapes.size(); s++)
    {
        const tinyobj::mesh_t& mesh = shapes[s].mesh;

        for(size_t i = 0; i < mesh.indices.size(); i++)
        {
            tinyobj::index_t index = mesh.indices[i];
            vertices.push_back(attrib.vertices[(index.vertex_index * 3)]);
            vertices.push_back(attrib.vertices[(index.vertex_index * 3) + 1]);
            vertices.push_back(attrib.vertices[(index.vertex_index * 3) + 2]);
            m_vertex_count++;
        }
    }

    return true;
}