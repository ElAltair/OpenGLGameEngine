#version 450 core

in vec3 Normal;
in vec3 FragPos;
out vec4 color;

struct Material{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

uniform Material material;

struct Light{
   
   //vec3 position;
   vec3 direction;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

uniform Light light;

uniform vec3 viewPos;

void main()
{

// Ambient
vec3 ambient = light.ambient * material.ambient;

//Difuse
vec3 norm = normalize(Normal);
//vec3 lightDir = normalize(light.position - FragPos);
// For directional light
vec3 lightDir = normalize(-light.direction);
float diff = max(dot(norm,lightDir),0.0f);
vec3 diffuse = light.diffuse * (material.diffuse * diff);

//Specular
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir,norm);
float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
vec3 specular =  light.specular * (material.specular * spec);

// Result color
vec3 resultColor = diffuse + ambient + specular;
color = vec4(resultColor, 1.0f);


}