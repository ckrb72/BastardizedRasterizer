#include <iostream>

#include <graphics.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "../src/math/lnal.h"
#include <cassert>
#include <cmath>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "../src/api/Mesh.h"
#include "../src/api/Camera.h"
#include "../src/api/Texture.h"

const char* vertex_shader_source = "#version 330 core\n"
"layout(location = 0) in vec3 a_pos;\n"
"layout(location = 1) in vec2 a_texCoord;\n"
"out vec2 texCoords;"
"uniform mat4 projection;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"void main()\n"
"{\n"
"gl_Position = projection * view * model * vec4(a_pos, 1.0);\n"
"texCoords = a_texCoord;\n"
"}\n\0";

const char* fragment_shader_source = "#version 330 core\n"
"in vec2 texCoords;\n"
"out vec4 color;\n"
"uniform sampler2D containerTexture;\n"
"void main()\n"
"{\n"
"color = texture(containerTexture, texCoords);\n"
"}\n\0";

bool wireframe = false;

void mouse_motion(void* save_data)
{
    std::cout << "Motion" << std::endl;
}

void mouse_button(void* save_data)
{
    if(!wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        wireframe = true;
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        wireframe = false;
    }
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
    if(!reader.ParseFromFile("./assets/model/utah_teapot.obj", reader_config))
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

            /*if(index.vertex_index >= 0)
            {
                shape_vertices.push_back(attrib.normals[(index.normal_index * 3)]);
                shape_vertices.push_back(attrib.normals[(index.normal_index * 3) + 1]);
                shape_vertices.push_back(attrib.normals[(index.normal_index * 3) + 2]);
            }

            if(index.texcoord_index >= 0)
            {
                shape_vertices.push_back(attrib.texcoords[(2 * index.texcoord_index)]);
                shape_vertices.push_back(attrib.texcoords[(2 * index.texcoord_index) + 1]);
            }*/

            num_vertices++;
        }
    }



    //Init Stage of Engine
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("Cube Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
    assert(window != nullptr);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_CreateContext(window);

    #ifndef __APPLE__
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    #endif

    Texture t{};
    if(!t.load_texture("./assets/img/container.jpg"))
    {
        std::cout << "Failed to load" << std::endl;
        return -1;
    }

    Mesh m{};
    m.load_model("./assets/model/ico-sphere.obj", MESH_ALL);

    float vertices[] = 
    {
        -0.5, -0.5, 0.0,    0.0, 0.0,
        0.5, -0.5, 0.0,     1.0, 0.0,
        0.5, 0.5, 0.0,      1.0, 1.0,
        -0.5, 0.5, 0.0,     0.0, 1.0
    };

    unsigned int indices[] = 
    {
        0, 1, 2,
        2, 3, 0
    };

    //End of init stage for now

    //OpenGL stuff now

    uint32_t vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * shape_vertices.size(), shape_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    //Shader stuff

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
        delete[] buffer;
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
        delete[] buffer;
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
        delete[] buffer;
    }

    glDetachShader(program ,v_shader_source);
    glDetachShader(program, f_shader_source);

    glDeleteShader(v_shader_source);
    glDeleteShader(f_shader_source);

    glUseProgram(program);



    //Create Camera for Scene
    Camera cam{};

    cam.gen_perspective(PI / 2, (float)(1920.0f / 1080.0f), 0.1, 10.0);

    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, cam.get_projection());

    //End of OpenGL stuff

    bool quit = false;
    lnal::mat4 model(1.0);
    lnal::vec3 s_factor(0.5, 0.5, 0.5);
    //lnal::scale(model, s_factor);
    lnal::vec3 translate(0.0, -1.0, 0.0);
    //lnal::translate_relative(model, translate);

    lnal::mat4 rotation;
    lnal::vec3 axis(0.0, -1.0, 0.0);
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
                        mouse_button_callback((void*)0);

                case SDL_KEYDOWN:
                    if(keydown_callback)
                        //keydown_callback((void*)0);
                default: 
                    break;
            }
        }

        model = rotation * model;

        x += 0.001;

        cam.look_at(lnal::vec3(0.0, 0.0, 3.0), lnal::vec3(0.0, 0.0, 0.0), lnal::vec3(0.0, 1.0, 0.0));


        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, model.data());

        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, cam.get_view());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.3, 0.3, 0.3, 1.0);

        t.bind(0);


        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);
        //glDrawArrays(GL_TRIANGLES, 0, num_vertices);
        SDL_GL_SwapWindow(window);
    }

    //Cleanup

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);

    //SDL_GL_DeleteContext(window);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}