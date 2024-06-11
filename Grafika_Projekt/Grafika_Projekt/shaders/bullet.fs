#version 460 core

in vec3 bulletColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(bulletColor, 1.0);
}