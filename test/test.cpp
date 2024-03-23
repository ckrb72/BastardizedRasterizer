#include <iostream>
#include "../src/math/lnal.h"
#include <math.h>

int main()
{
    lnal::vec3 a(1, 1, 1);

    a.print();

    a.normalize();

    a.print();

    lnal::vec3 b(1, 1, 1);
    lnal::vec3 c(1, 1, 1);

    lnal::vec3 d = b - (2 * b);

    d += lnal::vec3(2, 2, 2) * 45;

    lnal::vec3 e(10, 10, 10);

    e *= 0.1;

    d.print();

    e.print();

}