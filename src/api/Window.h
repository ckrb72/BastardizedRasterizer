#pragma once
#include <iostream>

#include <SDL2/SDL.h>

class Window
{
private:

    SDL_Window* m_window;
    size_t m_width;
    size_t m_height;
    const std::string m_title;

public:
    Window();

    Window(const std::string& title, int width, int height);

    ~Window();


};