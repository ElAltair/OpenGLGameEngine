#version 450 core

struct Material{
   sampler2D diffuseTexture;
  //vec3 ambient;
  //vec3 diffuse;
  //vec3 specular;
  sampler2D specularTexture;
  float shininess;
};

struct Material2{
  //sampler2D diffuseTexture;
  //vec3 ambient;
  //vec3 diffuse;
  //vec3 specular;
  float shininess;
};


struct PointLight{
   
   vec3 position;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float constant;
   float linear;
   float quadratic;

};

struct DirectionalLight{
    vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform sampler2D shadowMap;
uniform PointLight pointLight;
uniform DirectionalLight dirLight;
uniform Material dsmaterial;
uniform Material2 material;
uniform float isSpecular;
uniform vec3 viewPos;

in VShader_out 
{
   vec3 Normal;
   vec3 FragPos;
   vec2 TextureCoord;
   vec4 FragPosLightSpace;
} fs_in;

out vec4 color;


vec3 calculatePointLight(PointLight light, vec3 FragPos, vec3 Normal,vec3 lightDir);
vec3 calculateDirectionalLight(DirectionalLight light,vec3 lightDir, vec3 FragPos, vec3 Normal, vec3 viewDir);

float ShadowCalculation(vec4 FragPosLightSpace,vec3 Normal, vec3 lightDir)
{

vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;

if(projCoords.z > 1.0)
return 0.0;

projCoords = projCoords * 0.5 + 0.5;
float closestDepth = texture(shadowMap,projCoords.xy).r;
float currentDepth = projCoords.z;
float bias = max(0.05 * (1.0 - dot(Normal,lightDir)),0.005);
float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
return shadow;

}


void main()
{

vec3 lightDir = normalize(-dirLight.direction);
vec3 norm = normalize(fs_in.Normal);
vec3 viewDir = normalize(viewPos - fs_in.FragPos);


//vec3 result = calculatePointLight(pointLight,fs_in.FragPos,norm,viewDir);
vec3 result = calculateDirectionalLight(dirLight,lightDir,fs_in.FragPos,norm,viewDir);
vec3 ambient = pointLight.ambient * vec3(texture(dsmaterial.diffuseTexture,fs_in.TextureCoord));
ambient *= 0.2;

float shadow = ShadowCalculation(fs_in.FragPosLightSpace,norm,lightDir);
vec3 lighting = ambient + (1.0 - shadow) * result;
//vec3 lighting = ambient + result;
/*
if(gl_FrontFacing)
result =   calculatePointLight(pointLight,lightDir,fs_in.FragPos,norm,viewDir);
else
result = ambient * 0.7;
*/
//result +=ambient;



vec3 gamaResult = pow(lighting,vec3(1.0/2.2));
color = vec4(gamaResult,1.0);
}

vec3 calculateDirectionalLight(DirectionalLight light,vec3 lightDir, vec3 FragPos, vec3 Normal, vec3 viewDir)
{



vec3 halfwayDir = normalize(lightDir + viewDir);


float diff = max(dot(Normal,lightDir),0.0);
float spec = pow(max(dot(Normal,halfwayDir),0.0),dsmaterial.shininess);

vec3 diffuse = light.diffuse * diff * vec3(texture(dsmaterial.diffuseTexture,fs_in.TextureCoord));
//diffuse = light.diffuse;

vec3 specular = light.specular * spec * vec3(texture(dsmaterial.specularTexture,fs_in.TextureCoord));


return (diffuse + specular);

}


vec3 calculatePointLight(PointLight light, vec3 FragPos, vec3 Normal, vec3 viewDir)
{

vec3 lightDir = normalize(light.position - FragPos);
vec3 halfwayDir = normalize(lightDir + viewDir);


float diff = max(dot(Normal,lightDir),0.0);
float spec = pow(max(dot(Normal,halfwayDir),0.0),dsmaterial.shininess);

vec3 diffuse = light.diffuse * diff * vec3(texture(dsmaterial.diffuseTexture,fs_in.TextureCoord));
//diffuse = light.diffuse;
vec3 specular = light.specular * spec * vec3(texture(dsmaterial.specularTexture,fs_in.TextureCoord));

float distance = length(light.position - FragPos);
float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * distance * distance );

diffuse *= attenuation;
specular *= attenuation * 0.5;

return (diffuse + specular);
}
