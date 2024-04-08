#pragma once

#define PI 3.14159265359


namespace lnal
{

    class vec3 
    {

    private:

        float m_data[3];

    public:

        vec3();

        vec3(float x, float y, float z);


        //Operator overload methods
        /* 
            Don't need to make these operator overloads friends because
            it doesn't matter what side is the "parent" and what side 
            is interpreted as rhs if we are adding two vectors.
        */
        vec3 operator+(const vec3& rhs);
        vec3 operator*(const float scalar);
        vec3 operator-(const vec3& rhs);
        

        vec3& operator+=(const vec3& rhs);
        vec3& operator*=(const vec3& rhs);
        vec3& operator-=(const vec3& rhs);
        vec3& operator*=(float scalar);

        float& operator[](int index);

        //Const version of [] overload for when we pass in const vecs to functions
        const float& operator[](int index) const;


        //Need to make these friends so we can have on the scalar on both sides of the operator
        friend vec3 operator*(float scalar, const vec3& vec);
        //friend vec3 operator*(const vec3& vec, float scalar);

        //want to implement [] operator
        
        //Additional vector functionality

        void normalize();

        void print();

    };

    class mat4
    {
    private:

        float m_data[4][4];

    public:

        mat4() {};

        mat4(float a);

        //Operator overloading

        mat4 operator*(const mat4& rhs);
        mat4 operator+(const mat4& rhs);
        mat4 operator-(const mat4& rhs);

        mat4& operator+=(const mat4& rhs);
        mat4& operator-=(const mat4& rhs);

        friend mat4 operator*(float scalar, const mat4& mat);


        //Additional matrix functionality
        friend void gen_perspective_proj(mat4& A, float fovx, float aspect_ratio, float near, float far);
        friend void gen_orthographic_proj(mat4& A, float left, float right, float bottom, float top, float near, float far);
        friend void lookat(mat4& A, vec3 cam_pos, vec3 cam_lookat, vec3 temp_up);

        friend void translate_relative(mat4& A, const vec3& vec);
        friend void translate_absolute(mat4& A, const vec3& vec);
        friend void scale(mat4& A, const vec3& vec);
        friend void rotate(mat4& A, const vec3& axis, float angle);


        float* data();

        mat4 transpose();
        mat4 inverse();

        void print();

    };

    //Will need to implement these features but might want to make these methods of mat4 class instead of standalone fuctions
    void translate_relative(mat4& A, const vec3& vec);
    void translate_absolute(mat4& A, const vec3& vec);
    void scale(mat4& A, const vec3& vec);
    void rotate(mat4& A, const vec3& axis, float angle);

    void gen_perspective_proj(mat4& A, float fovx, float aspect_ratio, float near, float far);
    void gen_orthographic_proj(mat4& A, float left, float right, float bottom, float top, float near, float far);
    void lookat(mat4& A, vec3 cam_pos, vec3 cam_lookat, vec3 temp_up);

    vec3 cross(const vec3& a, const vec3& b);
    float dot(const vec3& a, const vec3& b);

    float radians(float degrees);

}