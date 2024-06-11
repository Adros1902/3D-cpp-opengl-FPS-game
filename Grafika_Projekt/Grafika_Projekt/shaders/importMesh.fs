#version 460 core

in vec3 triColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightColor;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int currentUsedLights;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

uniform Material colorForBullets;
  
uniform Material material;

struct Light{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

//flashlight

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

uniform SpotLight spotLight;
uniform PointLight lightBulb;
uniform Material bulbMaterial;
uniform bool flashlightOn;

#define NR_POINT_LIGHTS 32
uniform PointLight pointLights[NR_POINT_LIGHTS];


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material myMaterial);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir); //flashlight




void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    vec3 pointLightResult;
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        if (i < currentUsedLights) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, colorForBullets);
        }
    }
    result += CalcPointLight(lightBulb, norm, FragPos, viewDir, bulbMaterial);
    if(flashlightOn){
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    }
    vec3 texDiffuse1 = texture(texture_diffuse1, TexCoord).rgb;
    //vec3 texDiffuse2 = texture(texture_diffuse2, TexCoord).rgb;
    //vec3 texDiffuse3 = texture(texture_diffuse3, TexCoord).rgb;

    result = result * texDiffuse1;
    FragColor = vec4(result, 1.0);
}



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    //diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    //specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //resluts
    vec3 ambient = light.ambient * material.ambient * vec3(texture(texture1, TexCoord).rgb);
    vec3 diffuse = light.diffuse * diff * material.diffuse * vec3(texture(texture1, TexCoord).rgb);
    vec3 specular = light.specular * spec * material.specular * vec3(texture(texture1, TexCoord).rgb);
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material myMaterial)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), myMaterial.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * myMaterial.ambient;
    vec3 diffuse  = light.diffuse  * diff * myMaterial.diffuse;
    vec3 specular = light.specular * spec * myMaterial.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * material.ambient  * vec3(texture(texture1, TexCoord).rgb);
    vec3 diffuse = light.diffuse * diff * material.diffuse * vec3(texture(texture1, TexCoord).rgb);
    vec3 specular = light.specular * spec * material.specular *vec3(texture(texture1, TexCoord).rgb);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
