#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <GL/glew.h>


/* GLCall macro active on _DEBUG builds only, checks for GL errors and halts execution and prints info to the console if it finds any errors. */
#ifdef _DEBUG
#define GLCall(x) \
                GLClearError();\
                x;\
                assert((GLLogCall(#x, __FILE__, __LINE__)))
#else
#define GLCall(x) x
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* filePath, int line);

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};