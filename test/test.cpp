#include <iostream>
#include "../src/math/lnal.h"
#include <math.h>

int main()
{
    lnal::mat4 A(1);

    float* a_data = A.data();
    for(int i = 0; i < 16; i++)
    {
        a_data[i] = 15 - i;
    }

    A.print();

    lnal::mat4 B;

    float* data = B.data();

    for(int i = 0; i < 16; i++)
    {
        data[i] = i;
    }

    B.print();

    lnal::mat4 C = A * B;
    C.print();
}