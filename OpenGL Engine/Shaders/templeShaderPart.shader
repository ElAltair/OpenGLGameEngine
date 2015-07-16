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
}

struct FlashLight
{
   vec3 direction;
   float InnerCutoff;
   float OuterCutoff;

   vec3 position;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float linear;
   float quadratic;
   float constant;
}
vec3 calculateDirectionalLight(DirectionalLight light,vec3 normal, vec3 viewDir)
{

vec3 lightDir = normalize(-light.direction);

//Diffuse color
float diff = max(dot(normal,lightDir),0.0f);

//Specular color
vec3 reflectDir = reflect(-lightDir,normal);
float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

vec3 ambient = light.ambient * vec3(texture(material.diffuseTexture, fs_in.TextureCoord));
vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseTexture, fs_in.TextureCoord));
vec3 specular = light.specular * spec /* vec3(texture(material.specularTexture, fs_in.TextureCoord))*/;
return (ambient + diffuse + specular);

}
vec3 calculatePointLight(PointLight light,vec3 normal,vec3 fragPos, vec3 viewDir)
{

vec3 lightDir = normalize(light.position - fragPos);

//Diffuse color
float diff = max(dot(normal,lightDir),0.0f);

//Specular color
vec3 reflectDir = reflect(-lightDir,normal);
float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

// Attenuation

float distance = length(light.position - fragPos);
float attenuation = 1.0f /( light.constant + light.linear * distance + light.quadratic * (distance * distance));

vec3 ambient = light.ambient * vec3(texture(material.diffuseTexture, fs_in.TextureCoord));
vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseTexture, fs_in.TextureCoord));
vec3 specular = light.specular * spec /* vec3(texture(material.specularTexture, fs_in.TextureCoord))*/;

ambient *= attenuation;
diffuse *= attenuation;
specular *= attenuation;

return (ambient + diffuse + specular);

}

vec3 calculateFlashLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 viewDir
{

//Ambient
vec3 ambient = light.ambient * vec3(texture(material.diffusetexture, fs_in.texturecoord));

//Diffuse color
vec3 lightDir = normalize(light.position - fragPos);
float diff = max(dot(normal,lightdir),0.0f);
vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffusetexture, fs_in.texturecoord));

//Specular color
vec3 reflectdir = reflect(-lightdir,normal);
float spec = pow(max(dot(viewdir,reflectdir),0.0),material.shininess);
vec3 specular = light.specular * spec /* vec3(texture(material.speculartexture, fs_in.texturecoord))*/;

//Spotlight (soft edges)
float theta = dot(lightDir,normalize(-light.direction));
flaot epsilon = (light.InnerCutoff - light.OuterCutoff);
float intensity = clamp((theta - light.OuterCutonff) / epsilon , 0.0, 1.0);
diffuse *=intensity;
specular *=intensity;

//Attenuation
float distance = length(light.position - fragpos);
float attenuation = 1.0f /( light.constant + light.linear * distance + light.quadratic * (distance * distance));

ambient *= attenuation;
diffuse *= attenuation;
specular *= attenuation;

return (ambient + diffuse + specular);

}