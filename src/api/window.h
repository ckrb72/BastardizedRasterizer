/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                Window Class implementation that acts as a wrapper around any winodw libraries we are going to use (SDL or GLFW) so it can easily be swapped out

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */

#pragma once

/*
Can add #defines later for what library we want to use:
i.e. #define WINDOW_GLFW_API or #define WINDOW_SDL_API
*/
//Might want to put this include somewhere else
#include <glad/glad.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>

class window
{
private:

    SDL_Window* m_win_handle;
    bool m_close = false;

public:

    window(const std::string& title, int width, int height);
    ~window();

    bool should_close();
    void swap_buffer();

    void poll_events();

};