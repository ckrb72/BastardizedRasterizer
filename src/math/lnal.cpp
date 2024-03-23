

#include "lnal.h"
#include <math.h>


namespace lnal
{

    //Converts degrees to radians
    //@param degrees value to convert
    float radians(float degrees)
    {
        return (degrees * PI) / 180.0;   
    }

}