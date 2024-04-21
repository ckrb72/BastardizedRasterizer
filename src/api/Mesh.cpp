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

//Loads the model given the filepath
//@param filepath Where to find the model
//@param flags What to load from the model (Positions, Normals, Tex Coords, Colors)
bool Mesh::load_model(const std::string& filepath, uint32_t flags)
{
    //First need to check if buffers are already allocated. If so then delete them
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./assets/model";
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

    std::vector<float> vertices{};

    size_t num_vertices = 0;


    //Will want to update this loop to make sure it actually gets normals and colors and stuff
    for(size_t s = 0; s < shapes.size(); s++)
    {
        const tinyobj::mesh_t &mesh = shapes[s].mesh;
        for(int i = 0; i < mesh.indices.size(); i++)
        {
            tinyobj::index_t index = mesh.indices[i];

            //Positions
            if(flags & MESH_POSITIONS)
            {
                vertices.push_back(attrib.vertices[index.vertex_index * 3]);
                vertices.push_back(attrib.vertices[(index.vertex_index * 3) + 1]);
                vertices.push_back(attrib.vertices[(index.vertex_index * 3) + 2]);
            }

            //Hacky fix for now but will hopefully fix this later
            //If we asked for normals but they don't exist, mask out
            //the bit for MESH_NORMALS so we don't put that in our stride
            //when allocating buffers 
            /*if(index.normal_index < 0 && (flags & MESH_NORMALS))
            {
                flags &= ~MESH_NORMALS;
            }*/
            
            //Normals
            if(index.normal_index >= 0 && (flags & MESH_NORMALS))
            {
                vertices.push_back(attrib.normals[(3 * index.normal_index)]);
                vertices.push_back(attrib.normals[(3 * index.normal_index) + 1]);
                vertices.push_back(attrib.normals[(3 * index.normal_index) + 2]);
            }

            //Hacky fix for now but will hopefully fix this later
            //If we asked for texture coordinates but they don't exist, mask out
            //the bit for MESH_TEXCOORDS so we don't put that in our stride
            //when allocating buffers 
            /*if(index.texcoord_index < 0 && (flags & MESH_TEXCOORDS))
            {
                flags = flags & ~MESH_TEXCOORDS;
            }*/

            //Texture Coordinates
            if(index.texcoord_index >= 0 && (flags & MESH_TEXCOORDS))
            {
                vertices.push_back(attrib.normals[(2 * index.texcoord_index)]);
                vertices.push_back(attrib.normals[(2 * index.texcoord_index) + 1]);
            }

            num_vertices++;
        }
    }

    //Just putting this here for now. Will definitely need to move this later
    std::vector<unsigned int> indices;

    gen_buffers(vertices, indices, flags);

    return true;
}

//Generates the buffers on the GPU that hold the model
//@param vertices The vertices of the model
//@param indices The indices of the model
//@param flags What is contained in each vertex (Position, Normal, Texture Coordinate, Color)
void Mesh::gen_buffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, uint32_t flags)
{
    //Generate buffers
    glGenVertexArrays(1, &m_vertex_array);
    glGenBuffers(1, &m_vertex_buffer);
    glGenBuffers(1, &m_index_buffer);

    //Bind the vertex array
    glBindVertexArray(m_vertex_array);

    //Send over buffer data
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    //FIXME NEED TO FIGURE OUT HOW TO SET VERTEX POINTERS

    //Unbind all buffers (Don't really need to do this but will leave this here for now)
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}