/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                Holds implementation of the various vec classes (vec2, vec3, etc.) as well as any general purpose vector function definitions.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */

#include "lnal.h"
#include <math.h>
#include <iostream>

namespace lnal
{

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    Vector Class Implementation - 
*/   
    //Initializes to zero vector <0, 0, 0>
    vec3::vec3()
    {
        m_data[0] = 0;
        m_data[1] = 0;
        m_data[2] = 0;
    }

    //Sets vector components to x, y, z, respectively
    //@param x first component
    //@param y second component
    //@param z third component
    vec3::vec3(float x, float y, float z)
    {
        m_data[0] = x;
        m_data[1] = y;
        m_data[2] = z;
    }

    //Normalizes vector
    void vec3::normalize()
    {
        float magnitude = sqrt((m_data[0] * m_data[0]) + (m_data[1] * m_data[1]) + (m_data[2] * m_data[2]));

        m_data[0] /= magnitude;
        m_data[1] /= magnitude;
        m_data[2] /= magnitude;
    }

    //Prints out vector in nice format for easy debugging
    void vec3::print()
    {
        std::cout << "< " << m_data[0] << ", " << m_data[1] << ", " << m_data[2] << " >" << std::endl;
    }


    //Operator overload definitions

    vec3 vec3::operator+(const vec3& rhs)
    {
        return vec3(m_data[0] + rhs.m_data[0], m_data[1] + rhs.m_data[1], m_data[2] + rhs.m_data[2]);
    }

    vec3 vec3::operator*(const vec3& rhs)
    {
        return vec3(m_data[0] * rhs.m_data[0], m_data[1] * rhs.m_data[1], m_data[2] * rhs.m_data[2]);
    }

    vec3 vec3::operator-(const vec3& rhs)
    {
        return vec3(m_data[0] - rhs.m_data[0], m_data[1] - rhs.m_data[1], m_data[2] - rhs.m_data[2]);
    }

    vec3& vec3::operator+=(const vec3& rhs)
    {
        m_data[0] += rhs.m_data[0];
        m_data[1] += rhs.m_data[1];
        m_data[2] += rhs.m_data[2];

        return *this;
    }

    vec3& vec3::operator*=(const vec3& rhs)
    {
        m_data[0] *= rhs.m_data[0];
        m_data[1] *= rhs.m_data[1];
        m_data[2] *= rhs.m_data[2];

        return *this;
    }

    vec3& vec3::operator-=(const vec3& rhs)
    {
        m_data[0] -= rhs.m_data[0];
        m_data[1] -= rhs.m_data[1];
        m_data[2] -= rhs.m_data[2];

        return *this;
    }

    vec3& vec3::operator*=(float scalar)
    {
        m_data[0] *= scalar;
        m_data[1] *= scalar;
        m_data[2] *= scalar;

        return *this;
    }

    float& vec3::operator[](int index)
    {
        return m_data[index];
    }
    

    //Friend function definitions

    vec3 operator*(float scalar, const vec3& vec)
    {
        return vec3(scalar * vec.m_data[0], scalar * vec.m_data[1], scalar * vec.m_data[2]);
    }

    vec3 operator*(const vec3& vec, float scalar)
    {
        return vec3(vec.m_data[0] * scalar, vec.m_data[1] * scalar, vec.m_data[2] * scalar);
    }

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    Other Vector Function Definitions - 
*/  

    //Takes the cross products of a and b in the form AxB.
    //@param a lhs of cross product
    //@param b rhs of cross product
    //@return vec3 result of AxB
    vec3 cross(vec3& a, vec3& b)
    {
        vec3 result;
        result[0] = (a[1] * b[2]) - (a[2] * b[1]);
        result[1] = -((a[0] * b[2]) - (a[2] * b[0]));
        result[2] = (a[0] * b[1]) - (a[1] * b[0]);
        return result; 
    }

}