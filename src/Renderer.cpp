#include "Renderer.h"
#include <iostream>


void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* filePath, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error][0x" << std::hex << error << "]: " << function << " at line: " << std::dec << line << "\n";
        return false;
    }
    return true;
}