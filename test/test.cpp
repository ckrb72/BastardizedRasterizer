#include <iostream>
#include "../src/math/lnal.h"
#include <math.h>

int main()
{
    lnal::mat4 m;
    lnal::gen_perspective_proj(m, PI / 2, (1920.0 / 1080.0), 0.1, 10.0);

    m.print();

}