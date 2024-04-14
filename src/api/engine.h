/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                                                                        Engine Class implementation

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */

#pragma once
#include <iostream>
#include "window.h"

class engine
{

private:
    window m_window;
    bool m_running = true;

    bool is_running();
    void poll_events();
    void render();

public:
    engine(const std::string& title, size_t width, size_t height);
    ~engine();

    void run();

};