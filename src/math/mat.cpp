/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                Provides implementation for Matrix classes (i.e. mat3, mat4, etc.) as well as any general purpose matrix function definitions.



    Memory - 
        All matrices are laid out in column-major order. This is very important to understand because of the implications it has on how data is accessed.
        All matrices will look as though they are transposed but are actually just laid out so each column comes after the other. This is opposed to how
        C normally lays out data (usually in row-major order).

        OpenGL requires that the data be in column-major order which is why we are laying memory out this way. If we were to use row-major ordering, we would
        need to transpose the matrices before sending the data to the GPU, which is costly (especially since there can be many matrices per frame).

        Ideally (if I did my job right), this should be unnoticable to the user of these functions and normal pre-multiplication can be used. The whole row vs column major
        ordering only has implications for the memory layout of the actual data and does not change the underlying math. The multiplication functionality source code will
        look a little weird but that's because of the column-major ordering.


    Memory Layout Example - 
        Since we are not using any dynamically allocated arrays with pointers and such, the 2D array that holds the data for our matrix class is actually just syntactic
        sugar. What the compiler actually does is store the 4 x 4 2D array as a 1 x 16 1D array in memory. In C / C++, the convention with this is that each row of the array
        is stored in memory sequentially.

        For example, the 2D array:
        | 0, 1, 2 |     <- row 1
        | 3, 4, 5 |     <- row 2
        | 6, 7, 8 |     <- row 3

        Will be stored in memory as:
        [0, 1, 2, 3, 4, 5, 6, 7, 8]

        This convention that C / C++ uses is called row-major ordering since indexing is based on the rows.
        However, some other programming languages like to store the memory in what is called column-major ordering, which is when columns are layed out sequentially
        instead of rows.

        For example, the 2D array:
        | 0, 1, 2 |
        | 3, 4, 5 |
        | 6, 7, 8 |

        Would be stored in memory as:
        [0, 3, 6, 1, 4, 7, 2, 5, 8]

        The compiler of those other languages automatically does this for you so if we were using one of those langauges no conversion would be necessary.
        However, to do this in C / C++ we need to lay the array out a little bit differently, since the convention C / C++ uses is to automatically lay out
        each row of the array in memory sequentially. Since each row is layed out sequentially, we need to treat each row as if it were a column.

        For example take a look at the following array:
        | 0, 1, 2 |     <- Row 1
        | 3, 4, 5 |     <- Row 2
        | 6, 7, 8 |     <- Row 3

        As discussed before, this will be layed out as:
        [0, 1, 2, 3, 4, 5, 6, 7, 8]

        This is in row-major ordering but we want it to be layed out in column major ordering so we need to initialize the array as:
        | 0, 3, 6 |     <- Column 1
        | 1, 4, 7 |     <- Column 2
        | 2, 5, 8 |     <- Column 3

        The matrix now looks as if it was transposed (mathematically, we are still representing the original matrix though).
        Now each row of the array actually represents a column of the original matrix we were dealing with.
        The data is now layed out in memory as:
        [0, 3, 6, 1, 4, 7, 2, 5, 8]

        As you can see, now the data is in column major order and the correct layout we want.

        OpenGL expects everything in column major ordering so we will be doing all matrices like this.

        Again, none of this should affect that math so the operand ordering is still pre-multiplication (see Operand Ordering)


    Operand Ordering - 
        Pre-multiplication is the convention used in this library. This means the matrices further to the left will be applied last.

        For example, C * B * A will first apply B on A and then C on AB. In other words, if they were functions you could think of the composition C(B(A)) = CBA.
        
        Keep this in mind for all multiplication as messing up the order will give the wrong answer since matrix multiplication isn't commutative.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */

#include "lnal.h"
#include <iostream>

namespace lnal
{

    //Converts degrees to radians
    //@param degrees value to convert
    float radians(float degrees)
    {
        return (degrees * PI) / 180.0;   
    }

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    Matrix class implementation - 
*/    

    //Sets Diagonal of Matrix to a.
    //Rest of matrix guaranteed to be 0
    //@param a Value to set diagonal to
    mat4::mat4(float a)
    {
        m_data[0][0] = a;
        m_data[1][1] = a;
        m_data[2][2] = a;
        m_data[3][3] = a;

        m_data[0][1] = m_data[0][2] = m_data[0][3] = 0;
        m_data[1][0] = m_data[1][2] = m_data[1][3] = 0;
        m_data[2][0] = m_data[2][1] = m_data[2][3] = 0;
        m_data[3][0] = m_data[3][1] = m_data[3][2] = 0;
    }



    mat4 mat4::operator+(const mat4& rhs)
    {
        mat4 result{};

        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                result.m_data[i][j] = m_data[i][j] + rhs.m_data[i][j];
            }
        }

        return result;
    }

    mat4 mat4::operator-(const mat4& rhs)
    {
        mat4 result{};
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                result.m_data[i][j] = m_data[i][j] + rhs.m_data[i][j];
            }
        }

        return result;
    }

    mat4 operator*(float scalar, const mat4& rhs)
    {
        mat4 result{};

        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                result.m_data[i][j] = rhs.m_data[i][j] * scalar;
            }
        }

        return result;
    }

    mat4& mat4::operator+=(const mat4& rhs)
    {
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                m_data[i][j] += rhs.m_data[i][j];
            }
        }

        return *this;
    }

    mat4& mat4::operator-=(const mat4& rhs)
    {
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                m_data[i][j] -= rhs.m_data[i][j];
            }
        }

        return *this;
    }

    mat4 mat4::operator*(const mat4& rhs)
    {
        
    }

    void mat4::print()
    {
        for(int i = 0; i < 4; i++)
        {
            std::cout << "| " <<  m_data[i][0] << ", " << m_data[i][1] << ", " << m_data[i][2] << ", " << m_data[i][3] << " |" <<  std::endl;
        }
    }

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    Other Matrix Function Definitions - 
*/   

    //Creates a perspective projection matrix for rendering.
    //@param A the matrix to place the final result in
    //@param fovx horizontal fov for the camera
    //@param aspect_ratio aspect ratio of the projection (usually 16:9 [aka 1920 / 1080])
    //@param near near clipping plane of the projection
    //@param far far clipping plane of the projection
    void gen_perspective_proj(mat4 A, float fovx, float aspect_ratio, float near, float far) 
    {

    }

    //Creates an orthographic projection matrix for rendering
    //@param A the matrix to place the final result in
    //@param left left clipping plane
    //@param right right clipping plane
    //@param bottom bottom clipping plane
    //@param top top clipping plane
    //@param near near clipping plane
    //@param far far clipping plane
    void gen_orthographic_proj(mat4 A, float left, float right, float bottom, float top, float near, float far)
    {

    }

    //Generates the view matrix given a specified orientation
    //@param A the matrix to place the final result in
    //@param cam_pos position in world space of the camera
    //@param up position in world space of the up vector
    void lookat(mat4 A, vec3 cam_pos, vec3 cam_lookat, vec3 temp_up)
    {
        vec3 forward = cam_pos - cam_lookat;
        forward.normalize();

        temp_up.normalize();
        
        vec3 right = cross(temp_up, forward);
        right.normalize();

        vec3 up = cross(forward, right);
        up.normalize();


        //Make view matrix then return it
    }
}
