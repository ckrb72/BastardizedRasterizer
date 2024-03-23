/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                Provides implementation for Matrix classes (i.e. mat3, mat4, etc.) as well as any general purpose matrix function definitions.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */

#include "lnal.h"

namespace lnal
{

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
    void lookat(mat4 A, vec3 cam_pos, vec3 cam_lookat, vec3 up)
    {

    }
}
