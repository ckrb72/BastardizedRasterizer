#include "window.h"

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glad/glad.h>
#endif

window::window(const std::string& title, int width, int height)
{
    //Initialize SDL2
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "Failed to initialize SDL" << std::endl;
        return;
    }

    //Create window
    m_win_handle = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

    if(!m_win_handle)
    {
        std::cerr << "Failed to create window!" << std::endl;
        SDL_Quit();
        return;
    }

    //Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    //Create the context so we can actually render
    SDL_GL_CreateContext(m_win_handle);

    //If not on apple then load function pointers
    #ifndef __APPLE__
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    #endif
}




window::~window()
{
    SDL_DestroyWindow(m_win_handle);
    SDL_Quit();
}

bool window::should_close()
{
    return m_close;
}

void window::swap_buffer()
{
    SDL_GL_SwapWindow(m_win_handle);
}

//Will get rid of this later
void window::poll_events()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                m_close = true;
                break;
            //case SDL_MOUSEMOTION:
            //     mouse_motion_callback(/*stuff to pass to mouse motion callback*/)

            default:
                break;
        }
    }
}

void window::clear()
{
    glClearColor(0.3, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}