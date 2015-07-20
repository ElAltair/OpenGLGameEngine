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
float SampleShadowMap(sampler2D shadowMap, vec2 coords, float compare);
float SampleShadowMapLinear(sampler2D shadowMpa, vec2 coords, float compare);


float SampleShadowMap(sampler2D shadowMap, vec2 coords, float compare)
{

return step(compare,texture2D(shadowMap,coords.xy).r);

}

float SampleShadowMapLinear(sampler2D shadowMap, vec2 coords, float compare)
{

vec2 texelSize = vec2(1.0f/1024.0f, 1.0f/1024.0f);

vec2 pixelPos = coords/texelSize + vec2(0.5);
vec2 fracPart = fract(pixelPos);
vec2 startTexel = (pixelPos - fracPart) * texelSize;

float blTexel = SampleShadowMap(shadowMap,startTexel,compare);
float brTexel = SampleShadowMap(shadowMap,startTexel + vec2(texelSize.x,0.0), compare);
float tlTexel = SampleShadowMap(shadowMap,startTexel + vec2(0.0,texelSize.y), compare);
float trTexel = SampleShadowMap(shadowMap,startTexel + texelSize, compare);

float mixA = mix(blTexel, tlTexel, fracPart.y);
float mixB = mix(brTexel, trTexel, fracPart.y);

return mix(mixA, mixB, fracPart.x);
}

float SampleShadowMapPCF(sampler2D shadowMap, vec2 coords, float compare)
{

const float NUM_SAMPLES = 3.0f;
const float SAMPLES_START = (NUM_SAMPLES - 1.0f)/2.0f;
const float NUM_SAMPLES_SQUARED = NUM_SAMPLES * NUM_SAMPLES;


//vec2 texelSize = vec2(1.0f/1024.0f, 1.0f/1024.0f);
vec2 texelSize = 1.0f/textureSize(shadowMap,0);
float result = 0.0f;
	for(float y = -SAMPLES_START; y <= SAMPLES_START; y += 1.0f)
	{
		for( float x = -SAMPLES_START; x <= SAMPLES_START; x += 1.0f)
		{
	        vec2 coordsOffset = vec2(x,y) * texelSize;
			result += SampleShadowMapLinear(shadowMap, coords + coordsOffset ,compare);	
		}
	}

	return result/ NUM_SAMPLES_SQUARED;

}




float ShadowCalculation(vec4 FragPosLightSpace,vec3 Normal, vec3 lightDir)
{

vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;

if(projCoords.z > 1.0)
return 0.0;

projCoords = projCoords * 0.5 + 0.5;

float bias = (10.0f / 1024.0);
//float bias = max(0.05 * (1.0 - dot(Normal,lightDir)),0.005);

float shadow = SampleShadowMapPCF(shadowMap,projCoords.xy,projCoords.z - bias);

return shadow;

}


void main()
{

vec3 lightDir = normalize(-dirLight.direction);
vec3 norm = normalize(fs_in.Normal);
vec3 viewDir = normalize(viewPos - fs_in.FragPos);


//vec3 result = calculatePointLight(pointLight,fs_in.FragPos,norm,viewDir);


vec3 result = calculateDirectionalLight(dirLight,lightDir,fs_in.FragPos,norm,viewDir);
vec3 ambient = dirLight.ambient * vec3(texture(dsmaterial.diffuseTexture,fs_in.TextureCoord));
ambient *= 0.09;

float shadow = ShadowCalculation(fs_in.FragPosLightSpace,norm,lightDir);
vec3 lighting;

lighting = ambient + result * shadow;


if(gl_FrontFacing)
result =  lighting;
else
result = vec3(0.0f);

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
specular*= 0.5;

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
