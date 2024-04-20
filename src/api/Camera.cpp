#include "Camera.h"
#include "../math/lnal.h"


Camera::Camera()
:m_view(1.0), m_projection(1.0)
{}

Camera::~Camera()
{}

//Creates the perspective projection matrix for the camera
//@param fovx Horizontal FOV of Camera
//@param aspect_ratio Aspect Ratio of Screen
//@param near Near Plane of Projection
//@param far Far Plane of Projection
void Camera::gen_perspective(float fovx, float aspect_ratio, float near, float far)
{
    lnal::gen_perspective_proj(m_projection, fovx, aspect_ratio, near, far);
}

//Create the orthographic projection matrix for the camera
//@param left Left Plane of the Projection
//@param right Right Plane of the Projection
//@param bottom Bottom Plane of the Projection
//@param top Top Plane of the Projection
//@param near Near Plane of the Projection
//@param far Far Plane of the Projection
void Camera::gen_orthographic(float left, float right, float bottom, float top, float near, float far)
{
    lnal::gen_orthographic_proj(m_projection, left, right, bottom, top, near, far);
}

//Creates the LookAt Matrix for the Camera (Orientation of the Camera)
//@param cam_pos Position of the Camera in World Space
//@param cam_lookat Direction the Camera is Facing
//@param temp_up Temporary Vector Describing What 'Up' Is
void Camera::look_at(lnal::vec3 cam_pos, lnal::vec3 cam_lookat, lnal::vec3 temp_up)
{
    lnal::lookat(m_view, cam_pos, cam_lookat, temp_up);
}

//Returns pointer to first element of projection matrix
const float* Camera::get_projection()
{
    return (const float*)m_projection.data();
}

//Returns pointer to first element of view matrix
const float* Camera::get_view()
{
    return (const float*)m_view.data();
}