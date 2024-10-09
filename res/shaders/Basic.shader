#shader vertex
#version 330 core
        
//Receiving vertex attribute data
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

//'Varying' variable to pass data from vertex shader to fragment shader.
out vec2 v_TexCoord;

void main()
{
    gl_Position = position;
    v_TexCoord = texCoord;
};

#shader fragment
#version 330 core
        
out vec4 color;
in vec2 v_TexCoord;

//Uniforms get data from CPU into the shader (GPU)
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;
};