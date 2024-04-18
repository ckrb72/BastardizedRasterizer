#include <iostream>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glad/glad.h>
#endif

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "../src/math/lnal.h"
#include <cassert>
#include <cmath>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

const char* vertex_shader_source = "#version 330 core\n"
"layout(location = 0) in vec3 a_pos;\n"
"uniform mat4 projection;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"void main()\n"
"{\n"
"gl_Position = projection * view * model * vec4(a_pos, 1.0);\n"
"}\n\0";

const char* fragment_shader_source = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(0.5, 0.3, 0.5, 1.0);\n"
"}\n\0";

/*
- Textures
*/

void mouse_motion(void* save_data)
{
    std::cout << "Motion" << std::endl;
}

void mouse_button(void* save_data)
{
    std::cout << "Mouse button" << std::endl;
}

void keydown(void* save_data)
{
    std::cout << "KeyDown" << std::endl;
}

//Need to pass in void* or void** (possibly to a struct) that would allow the user to access data that persists across the function calls
//This way the user can actually do stuff with the data that they modify
static void (*mouse_callback)(void* save_data) = mouse_motion;
static void (*mouse_button_callback)(void* save_data) = mouse_button;
static void (*keydown_callback)(void* save_data) = keydown;

int main()
{

    //Load OBJ model

    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./";
    reader_config.triangulate = true;

    //Read OBJ in
    if(!reader.ParseFromFile("./ico-sphere.obj", reader_config))
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

    std::vector<float> shape_vertices(shapes[0].mesh.indices.size());

    for(size_t s = 0; s < shapes.size(); s++)
    {
        const tinyobj::mesh_t &mesh = shapes[s].mesh;
        for(int i = 0; i < mesh.indices.size(); i++)
        {
            tinyobj::index_t index = mesh.indices[i];
            shape_vertices.push_back(attrib.vertices[index.vertex_index * 3]);
            shape_vertices.push_back(attrib.vertices[index.vertex_index * 3 + 1]);
            shape_vertices.push_back(attrib.vertices[index.vertex_index * 3 + 2]);
        }
    }

    std::cout << shape_vertices.size() * sizeof(float) << std::endl;

    //Init Stage of Engine
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("Cube Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_OPENGL);
    assert(window != nullptr);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_CreateContext(window);

    #ifndef __APPLE__
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    #endif

    //End of init stage for now

    //OpenGL stuff now

    float vertices[] = 
    {
        -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0,
        0.5, 0.5, 0.0,
        -0.5, 0.5, 0.0,

        //-0.5, -0.5, -2.0,
        //0.5, -0.5, -2.0,
        //0.5, 0.5, -2.0,
        //-0.5, 0.5, -2.0
    };

    unsigned int indices[] = 
    {
        0, 1, 2,
        2, 3, 0
    };

    uint32_t vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    //glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * shape_vertices.size(), shape_vertices.data(), GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    //Error checking variables
    char buffer[255];
    int success;

    unsigned int v_shader_source = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_shader_source, 1, &vertex_shader_source, nullptr);
    glCompileShader(v_shader_source);

    glGetShaderiv(v_shader_source, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        int length;
        glGetShaderiv(v_shader_source, GL_INFO_LOG_LENGTH, &length);
        char* buffer = new char[length];
        glGetShaderInfoLog(v_shader_source, length,nullptr, buffer);
        std::cout << buffer << std::endl;
        delete buffer;
    }

    unsigned int f_shader_source = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_shader_source, 1, &fragment_shader_source, nullptr);
    glCompileShader(f_shader_source);

    glGetShaderiv(f_shader_source, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        int length;
        glGetShaderiv(f_shader_source, GL_INFO_LOG_LENGTH, &length);
        char* buffer = new char[length];
        glGetShaderInfoLog(f_shader_source, length,nullptr, buffer);
        std::cout << buffer << std::endl;
        delete buffer;
    }

    unsigned int program = glCreateProgram();
    glAttachShader(program, v_shader_source);
    glAttachShader(program, f_shader_source);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* buffer = new char[length];
        glGetProgramInfoLog(program, length, nullptr, buffer);
        std::cout << buffer << std::endl;
        delete buffer;
    }

    glDetachShader(program ,v_shader_source);
    glDetachShader(program, f_shader_source);

    glDeleteShader(v_shader_source);
    glDeleteShader(f_shader_source);

    glUseProgram(program);

    lnal::mat4 projection;

    lnal::gen_perspective_proj(projection, PI / 2, (float)(1920.0f/1080.0f), 0.1, 10.0);

    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, projection.data());

    //End of OpenGL stuff

    bool quit = false;
    lnal::mat4 model(1.0);

    lnal::mat4 rotation;
    lnal::vec3 axis(0.0, 0.0, 1.0);
    lnal::rotation_matrix(rotation, axis, PI / 1000);

    float x = 0.0;

    //Main Loop of engine
    while(!quit)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEMOTION:
                    if(mouse_callback)
                        //mouse_callback((void*)0);
                    break;
                
                case SDL_MOUSEBUTTONDOWN:
                    if(mouse_button_callback)
                        //mouse_button_callback((void*)0);

                case SDL_KEYDOWN:
                    if(keydown_callback)
                        //keydown_callback((void*)0);
                default: 
                    break;
            }
        }

        model = rotation * model;

        x += 0.001;

        lnal::mat4 view(1.0);
        lnal::lookat(view, lnal::vec3(x, 0.0, 3.0), lnal::vec3(0.0, 0.0, 0.0), lnal::vec3(0.0, 1.0, 0.0));


        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, model.data());

        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, view.data());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.3, 0.3, 0.3, 1.0);

        glBindVertexArray(vao);
        //glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, shape_vertices.size());
        SDL_GL_SwapWindow(window);
    }


    //Cleanup
    SDL_GL_DeleteContext(window);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}