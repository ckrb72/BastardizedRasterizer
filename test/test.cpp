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

    lnal::vec3 e(10, 1, 10);

    e *= 0.1;

    d.print();

    e.print();

    std::cout << e[1] << std::endl;

    lnal::mat4 matrix(1);

    lnal::mat4 m = 3 * matrix;

    m.print();


    lnal::vec3 x_axis(3.4, 7, 9.23);
    lnal::vec3 y_axis(1, 4, 3);

    lnal::vec3 z_axis = lnal::cross(x_axis, y_axis);

    z_axis.print();

}