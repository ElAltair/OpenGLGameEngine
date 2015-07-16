#version 450 core

struct Material{
  sampler2D diffuseTexture;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

uniform Material material;

struct DirectionalLight{
   
   //vec3 position;
   vec3 direction;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct PointLight{
   
   vec3 position;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float linear;
   float quadratic;
   float constant;
};

uniform DirectionalLight directLight;
uniform PointLight pointLight;

uniform vec3 viewPos;

in VShader_out 
{
   vec3 Normal;
   vec3 FragPos;
   vec2 TextureCoord;
} fs_in;

out vec4 color;




void main()
{
vec3 norm = normalize(fs_in.Normal);
vec3 viewDir = normalize(viewPos - fs_in.FragPos);

color = texture(material.diffuseTexture,fs_in.TextureCoord);
}

