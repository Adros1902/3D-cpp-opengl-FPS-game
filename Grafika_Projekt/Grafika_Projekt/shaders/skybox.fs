#version 460 core
out vec4 FragColor;

in vec3 TexCoords;

in vec3 skyColor;
uniform vec3 lightColor;
uniform samplerCube skybox;

void main()
{    
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = ambient * texture(skybox, TexCoords).rgb;
    FragColor = vec4(result, 1.0);
}