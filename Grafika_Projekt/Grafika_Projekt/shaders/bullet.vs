#version 460 core
layout (location = 0) in vec3 aPos1;
layout (location = 1) in vec3 aBulletColor;

out vec3 bulletColor;
out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos1, 1.0);
    bulletColor = aBulletColor;
}