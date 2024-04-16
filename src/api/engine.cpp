#include "engine.h"

engine::engine(const std::string& title, size_t width, size_t height) 
:m_window(title, width, height)
{

}

engine::~engine()
{
    //Nothing to do right now
}

void engine::run()
{
    
    while(is_running())
    {
        //time.march_forward();
        //event_handler.poll_events();
        //physics.blow_things_up();
        //renderer.render()     <- UI will be done in here as well even though we are going to make our own library


        poll_events();
        render();
    
    }
}

bool engine::is_running()
{
    return m_running;
}

//This will be done in the event handler (just in the engine class for now because those aren't implemented)
void engine::poll_events()
{
    //Would call event handler here but not implemented yet
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                m_running = false;
        }
    }
}

//This will be done in the renderer (just in engine class for now because those aren't implemented)
void engine::render()
{

    //Would call renderer right here but not implemented yet

    m_window.clear();

    m_window.swap_buffer();
}