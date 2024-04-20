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


    References for Further Learning - 
    
        Perspective Projection Matrix Derivation - https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/opengl-perspective-projection-matrix.html
        Orthographic Projection Matrix Derivation - https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/orthographic-projection-matrix.html
        View / Lookat Matrix Derivation - https://www.songho.ca/opengl/gl_camera.html
        Rotation Matrix Derivation - https://www.songho.ca/opengl/gl_rotate.html


------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */

#include "lnal.h"
#include <iostream>
#include <math.h>

namespace lnal
{

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


/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    Overloaded Operator Definitions - 
*/   
    mat4 mat4::operator+(const mat4& rhs)
    {
        mat4 result;

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
        mat4 result;
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
        mat4 result;

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

    //Can optimize this by transposing matrix A first (use cache correctly)
    //Matrix multiplication of B * A  ( as function compositions: B(A(X))  )
    mat4 mat4::operator*(const mat4& rhs)
    {
        mat4 result{};

        for(int c = 0; c < 4; c++)
        {
            for(int r = 0; r < 4; r++)
            {
                float sum = 0.0;

                //Dot product of row of A (looks like column because of mem layout)
                //and column of B (looks like row becuase of mem layout)
                //Allows us to do B * A with the column-major matrix layouts
                //(technically we are doing A * B, which is what we want because the matrices are
                //"transposed" [column-major order]). Laying the mult out like this allows us
                //to write B * A, which how multiplications are normally written.
                for(int i = 0; i < 4; i++)
                {
                              //B               //A
                    sum += m_data[i][r] * rhs.m_data[c][i];
                }
                result.m_data[c][r] = sum;
            }
        }


        return result;
    }

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    Other Matrix Function Definitions - 
*/   

    //Prints the matrix in a nice format
    void mat4::print()
    {
        std::cout << std::endl;
        for(int i = 0; i < 4; i++)
        {
            std::cout << "| " <<  m_data[i][0] << ", " << m_data[i][1] << ", " << m_data[i][2] << ", " << m_data[i][3] << " |" <<  std::endl;
        }
        std::cout << std::endl;
    }

    //Returns a pointer to the first element of the internal array
    float* mat4::data()
    {
        return &m_data[0][0];
    }

    //Creates a perspective projection matrix for rendering.
    //@param A the matrix to place the final result in
    //@param fovx horizontal fov for the camera
    //@param aspect_ratio aspect ratio of the projection (usually 16:9 [aka 1920 / 1080])
    //@param near near clipping plane of the projection
    //@param far far clipping plane of the projection
    void gen_perspective_proj(mat4& A, float fovx, float aspect_ratio, float near, float far) 
    {
        float right = tan(fovx / 2) * near;
        float left = -right;
        float top = right / aspect_ratio;
        float bottom = -top;

        A.m_data[0][0] = (2 * near) / (right - left);
        A.m_data[0][1] = 0;
        A.m_data[0][2] = 0;
        A.m_data[0][3] = 0;

        A.m_data[1][0] = 0;
        A.m_data[1][1] = (2 * near) / (top - bottom);
        A.m_data[1][2] = 0;
        A.m_data[1][3] = 0;

        A.m_data[2][0] = (right + left) / (right - left);
        A.m_data[2][1] = (top + bottom) / (top - bottom);
        A.m_data[2][2] = -((far + near) / (far - near));
        A.m_data[2][3] = -1;

        A.m_data[3][0] = 0;
        A.m_data[3][1] = 0;
        A.m_data[3][2] = -((2 * far * near) / (far - near));
        A.m_data[3][3] = 0;
    }

    //Creates an orthographic projection matrix for rendering
    //@param A the matrix to place the final result in
    //@param left left clipping plane
    //@param right right clipping plane
    //@param bottom bottom clipping plane
    //@param top top clipping plane
    //@param near near clipping plane
    //@param far far clipping plane
    void gen_orthographic_proj(mat4& A, float left, float right, float bottom, float top, float near, float far)
    {
        A.m_data[0][0] = 2 / (right - left);
        A.m_data[0][1] = 0;
        A.m_data[0][2] = 0;
        A.m_data[0][3] = 0;

        A.m_data[1][0] = 0;
        A.m_data[1][1] = 2 / (top - bottom);
        A.m_data[1][2] = 0;
        A.m_data[1][3] = 0;

        A.m_data[2][0] = 0;
        A.m_data[2][1] = 0;
        A.m_data[2][2] = -2 / (far - near);
        A.m_data[2][3] = 0;

        A.m_data[3][0] = -(right + left) / (right - left);
        A.m_data[3][1] = -(top + bottom) / (top - bottom);
        A.m_data[3][2] = -(far + near) / (far - near);
        A.m_data[3][3] = 1;
    }

    //Generates the view matrix given a specified orientation
    //@param A the matrix to place the final result in
    //@param cam_pos position in world space of the camera
    //@param up position in world space of the up vector
    void lookat(mat4& A, vec3 cam_pos, vec3 cam_lookat, vec3 temp_up)
    {
        vec3 forward = cam_pos - cam_lookat;
        forward.normalize();

        temp_up.normalize();
        vec3 right = cross(temp_up, forward);
        right.normalize();

        vec3 up = cross(forward, right);
        up.normalize();



        //Actual view matrix
        A.m_data[0][0] = right[0];
        A.m_data[0][1] = up[0];
        A.m_data[0][2] = forward[0];
        A.m_data[0][3] = 0;

        A.m_data[1][0] = right[1];
        A.m_data[1][1] = up[1];
        A.m_data[1][2] = forward[1];
        A.m_data[1][3] = 0;

        A.m_data[2][0] = right[2];
        A.m_data[2][1] = up[2];
        A.m_data[2][2] = forward[2];
        A.m_data[2][3] = 0;

        A.m_data[3][0] = -dot(right, cam_pos);
        A.m_data[3][1] = -dot(up, cam_pos);
        A.m_data[3][2] = -dot(forward, cam_pos);
        A.m_data[3][3] = 1;
    }

    //Translates matrix A along vector vec relative to current position
    //For example, if current position is (1, 1, 1) and you input vector
    // (2, 2, 2), the new position will be (3, 3, 3)
    //@param A matrix to translate
    //@param vec vector to translate along
    void translate_relative(mat4& A, const vec3& vec)
    {
        A.m_data[3][0] += vec[0];
        A.m_data[3][1] += vec[1];
        A.m_data[3][2] += vec[2];
    }

    //Translates matrix A to position described in vec
    //For example, if vec is (1, 1, 1), no matter what the
    //current position is, the final transform will be (1, 1, 1)
    //@param A matrix to translate
    //@param vec position to translate to
    void translate_absolute(mat4& A, const vec3& vec)
    {
        A.m_data[3][0] = vec[0];
        A.m_data[3][1] = vec[1];
        A.m_data[3][2] = vec[2];
    }

    //Scales matrix A in relation to the components of vec
    // For example,
    //      passing in (0.5, 1, 2) would scale x by 0.5,
    //      y by 1, and z by 2
    //@param A matrix to scale
    //@param vec holds components to scale against
    void scale(mat4& A, const vec3& vec)
    {
        A.m_data[0][0] *= vec[0];
        A.m_data[1][1] *= vec[1];
        A.m_data[2][2] *= vec[2];
    }

    //Encodes a rotation of specified radians about the axis specified into A
    //@param A matrix to rotate
    //@param axis arbitrary axis to rotate around
    //@param angle angle to rotate in radians
    void rotation_matrix(mat4& A, vec3& axis, float angle)
    {

        axis.normalize();

        float c = cos(angle);
        float s = sin(angle);

        float x = axis[0];
        float y = axis[1];
        float z = axis[2];

        A.m_data[0][0] = ((1 - c) * (x * x)) + c;
        A.m_data[0][1] = ((1 - c) * (x * y)) + (s * z);
        A.m_data[0][2] = ((1 - c) * (x * z)) - (s * y);
        A.m_data[0][3] = 0;

        A.m_data[1][0] = ((1 - c) * (x * y)) - (s * z);
        A.m_data[1][1] = ((1 - c) * (y * y)) + c;
        A.m_data[1][2] = ((1 - c) * (y * z)) + (s * x);
        A.m_data[1][3] = 0;

        A.m_data[2][0] = ((1 - c) * (x * z)) + (s * y);
        A.m_data[2][1] = ((1 - c) * (y * z)) - (s * x);
        A.m_data[2][2] = ((1 - c) * (z * z)) + c;
        A.m_data[2][3] = 0;

        A.m_data[3][0] = 0;
        A.m_data[3][1] = 0;
        A.m_data[3][2] = 0;
        A.m_data[3][3] = 1;
    }

    //Rotates matrix A around the given axis by the specified angle in radians
    //@param A matrix to rotate
    //@param axis arbitrary axis to rotate around
    //@param angle angle to rotate in radians
    void rotate(mat4& A, vec3& axis, float angle)
    {
        lnal::mat4 rotation;
        rotation_matrix(rotation, axis, angle);

        A = rotation * A;
    }
}
