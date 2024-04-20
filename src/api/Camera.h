#pragma once
#include "../math/lnal.h"


class Camera
{
private:

    //View Matrix (lookat)
    lnal::mat4 m_view;

    //Projection Matrix (Perspective or Orthographic)
    lnal::mat4 m_projection;

    //Might want to save both orthographic and perspective matricies and then swap them out when needed

public:
    Camera();

    ~Camera();


    //Creates the perspective projection matrix for the camera
    //@param fovx Horizontal FOV of Camera
    //@param aspect_ratio Aspect Ratio of Screen
    //@param near Near Plane of Projection
    //@param far Far Plane of Projection
    void gen_perspective(float fovx, float aspect_ratio, float near, float far);


    //Create the orthographic projection matrix for the camera
    //@param left Left Plane of the Projection
    //@param right Right Plane of the Projection
    //@param bottom Bottom Plane of the Projection
    //@param top Top Plane of the Projection
    //@param near Near Plane of the Projection
    //@param far Far Plane of the Projection
    void gen_orthographic(float left, float right, float bottom, float top, float near, float far);

    //Creates the LookAt Matrix for the Camera (Orientation of the Camera)
    //@param cam_pos Position of the Camera in World Space
    //@param cam_lookat Direction the Camera is Facing
    //@param temp_up Temporary Vector Describing What 'Up' Is
    void look_at(lnal::vec3 cam_pos, lnal::vec3 cam_lookat, lnal::vec3 temp_up);

    //Returns pointer to first element of projection matrix
    const float* get_projection();

    //Returns pointer to first element of view matrix
    const float* get_view();
};