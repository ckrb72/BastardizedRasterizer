#include <iostream>
#include "../src/math/lnal.h"
#include <math.h>

int main()
{

    lnal::vec3 a(1, 1, 1);

    lnal::vec3 b = a * 1.5;

    b.print();

    lnal::mat4 A(1);

    lnal::translate_relative(A, lnal::vec3(1, 2, 1));

    lnal::translate_relative(A, lnal::vec3(1, 1, 1));

    lnal::scale(A, lnal::vec3(4, 3, 2));
    A.print();
}