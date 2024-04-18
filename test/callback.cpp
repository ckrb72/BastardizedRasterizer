#include <iostream>
#include "../src/math/lnal.h"


//Callback pointers
//Could probably store all of the callbacks in an array to allow for more cache coherency (if one callback is called in a frame, it likely others will be too)
static void (*keyboard_callback)(int, int) = nullptr;
static void (*mouse_callback)(int, int) = nullptr;

//Callback defines
#define CALLBACK_INFO_NONE 0
#define CALLBACK_INFO_KEYBOARD 1
#define CALLBACK_INFO_MOUSE 2

//Callback registration function
static void register_callback(void (*callback)(int, int), uint32_t info);

//Users would never see these functions (just for testing purposes)
//Would be triggered when an event happened (could put this in the switch case stuff)
static void input_event(int a);
static void mouse_event(int a);

//User defined callback functions
static void call(int a, int b)
{
    std::cout << a + b << std::endl;
}

static void test(int a, int b)
{
    std::cout << "Testing..." << std::endl;
}


int main(int argv, char** args)
{
    //Actually register the callback
    register_callback(call, CALLBACK_INFO_KEYBOARD | CALLBACK_INFO_MOUSE);

    //This overrides the last registered callback
    //register_callback(test, 0);

    //Trigger the callback with an event
    input_event(100);

    mouse_event(100);

    //Will want to wrap this in a engine object or something like that.
    //Renderer object will contain all rendering code
    //window win("Test", 1920, 1080);

    /*while(!win.should_close())
    {

        //Engine loop:
        //handle_input()
        //render()
        //extra stuff for later

        //Need to figure out how to poll the events
        //This will probably be window specific events
        //Might want to decouple this from window class and make it it's own thing so we can handle input some other way.
        win.poll_events();

        win.clear();

        win.swap_buffer();
    }*/
}


//Need to figure out bitwise operators to use with flags
void register_callback(void (*call)(int, int), uint32_t info)
{

    if(CALLBACK_INFO_KEYBOARD & info)
        keyboard_callback = call;

    if(CALLBACK_INFO_MOUSE & info)
        mouse_callback = call;
}


void input_event(int a)
{
    std::cout << "In input event" << std::endl;
    if(keyboard_callback == nullptr)
        return;

    keyboard_callback(a, 10);
}

void mouse_event(int a)
{
    std::cout << "In mouse event" << std::endl;
    if(mouse_callback == nullptr)
        return;
    
    mouse_callback(100, 10);
}