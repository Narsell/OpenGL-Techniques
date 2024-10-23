#shader vertex
#version 330 core
        
//Receiving vertex attribute data
layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCoord;
layout(location = 3) in float a_texIndex;

//'Varying' variable to pass data from vertex shader to fragment shader.
out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * a_position;
    v_Color = a_color;
    v_TexCoord = a_texCoord;
    v_TexIndex = a_texIndex;
};

#shader fragment
#version 330 core
        
out vec4 o_Color;

//Getting data from vertex shader
in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;

//Array of textures IDs
uniform sampler2D u_Textures[2];

void main()
{
    bool displayTexture = bool(v_TexIndex);
    if(displayTexture)
    {
        int index = int(v_TexIndex) - 1;
        o_Color = texture(u_Textures[index], v_TexCoord);
    } else
    {
        o_Color = v_Color;
    }
};