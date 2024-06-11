#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 skyInColor;

out vec3 TexCoords;
out vec3 skyColor;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    skyColor = skyInColor;
    gl_Position = projection * view * vec4(aPos, 1.0);
} 