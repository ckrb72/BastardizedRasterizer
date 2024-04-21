#include "debug.h"

#include <graphics.h>
#include <iostream>
void glCheckError_(const char* file, int line)
{
    GLenum error;
    while((error = glGetError()) != GL_NO_ERROR)
    {
        std::string error_string;

        switch(error)
        {
            case GL_INVALID_ENUM:
                error_string = "Invalid Enum";
                break;

            case GL_INVALID_VALUE:
                error_string = "Invalid Value";
                break;

            case GL_INVALID_OPERATION:
                error_string = "Invalid Operation";
                break;

            case GL_OUT_OF_MEMORY:
                error_string = "Out of Memory";
                break;

            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error_string = "Invalid Framebuffer Operation";
                break;

            case GL_INVALID_INDEX:
                error_string = "Invalid Index";
                break;
        }

        std::cout << "GL Error: " << error_string << " | " << file << " (" << line << ")" << std::endl;
    }
}