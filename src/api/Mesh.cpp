#include "Mesh.h"

#include <iostream>

#include <tiny_obj_loader.h>
#include <glad/glad.h>


Mesh::Mesh()
:m_vertex_array(0), m_vertex_buffer(0), m_index_buffer(0), model(1.0), m_vertex_count(0)
{}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_vertex_buffer);
    glDeleteBuffers(1, &m_index_buffer);
    glDeleteVertexArrays(1, &m_vertex_array);
}

bool Mesh::load_file(const std::string& filepath)
{
    //First need to check if buffers are already allocated. If so then delete them

    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./";
    reader_config.triangulate = true;

    //Read OBJ in
    if(!reader.ParseFromFile(filepath, reader_config))
    {
        if(!reader.Error().empty())
        {
            std::cerr << reader.Error();
        }

        exit(1);
    }

    if(!reader.Warning().empty())
    {
        std::cout << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    std::vector<float> shape_vertices{};

    size_t num_vertices = 0;


    //Will want to update this loop to make sure it actually gets normals and colors and stuff
    for(size_t s = 0; s < shapes.size(); s++)
    {
        const tinyobj::mesh_t &mesh = shapes[s].mesh;
        for(int i = 0; i < mesh.indices.size(); i++)
        {
            tinyobj::index_t index = mesh.indices[i];

            //Positions
            shape_vertices.push_back(attrib.vertices[index.vertex_index * 3]);
            shape_vertices.push_back(attrib.vertices[(index.vertex_index * 3) + 1]);
            shape_vertices.push_back(attrib.vertices[(index.vertex_index * 3) + 2]);
            
            //Normals
            //Need if statement here because some might not have normals
            //shape_vertices.push_back(attrib.normals[(index.normal_index * 3)]);
            //shape_vertices.push_back(attrib.normals[(index.normal_index * 3) + 1]);
            //shape_vertices.push_back(attrib.normals[(index.normal_index * 3) + 2]);

            num_vertices++;
        }
    }

    //Just putting this here for now. Will definitely need to move this later
    std::vector<unsigned int> indices;

    //FIXME: THIS IS BUSTED RIGHT NOW SO DON'T USE IT
    //gen_buffers(shape_vertices, indices);

    return true;
}

void Mesh::gen_buffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
{
    //Generate buffers
    glGenVertexArrays(1, &m_vertex_array);
    glGenBuffers(1, &m_vertex_buffer);
    //glGenBuffers(1, &m_index_buffer);

    //Bind the vertex array
    glBindVertexArray(m_vertex_array);

    //Send over buffer data
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


    //Unbind all buffers (Don't really need to do this but will leave this here for now)
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}