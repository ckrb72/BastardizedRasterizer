#include "../src/api/engine.h"
#include "../src/math/lnal.h"
#include <iostream>

//Feels very C++ to me...
//Might want to retreat back to the comforts of pure C
//Should separate logic from data. (Structs and functions work perfectly for this... OOP seems to be the root of all evils)

int main()
{
    //Don't like how this is a class
    //Would like it better if this was a state struct with an init function
    engine e("Engine Test", 1920, 1080);

    e.run();
}