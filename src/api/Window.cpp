#include "Window.h"

#include <glad/glad.h>
#include <cassert>

Window::Window()
{}

Window::Window(const std::string& title, int width, int height)
:m_width(width), m_height(height), m_title(title)
{

    //Might want to move this into a different function so we can easily check if SDL failed to initialize
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to init SDL: " << SDL_GetError() << std::endl;
    }

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

    assert(m_window != nullptr);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_CreateContext(m_window);

    #ifndef __APPLE__
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    #endif
}

Window::~Window()
{
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}