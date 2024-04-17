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

const char* vertex_shader_source = "#version 330 core\n"
"layout(location = 0) in vec3 a_pos;\n"
"uniform mat4 projection;\n"
"uniform mat4 model;\n"
"void main()\n"
"{\n"
"gl_Position = projection * vec4(a_pos, 1.0);\n"
"}\n\0";

const char* fragment_shader_source = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(0.5, 0.3, 0.5, 1.0);\n"
"}\n\0";

/*
- Texture
*/

int main()
{

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
        -0.5, -0.5, -1.0,
        0.5, -0.5, -1.0,
        0.5, 0.5, -1.0,
        -0.5, 0.5, -1.0
    };

    unsigned int indices[] = 
    {
        0, 1, 2,
        2, 3, 0
    };

    uint32_t vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


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

    projection.print();

    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, projection.data());

    //End of OpenGL stuff

    bool quit = false;

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
                default: 
                    break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.3, 0.3, 0.3, 1.0);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);

        SDL_GL_SwapWindow(window);
    }


    //Cleanup
    SDL_GL_DeleteContext(window);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}