#version 450 core


out vec4 color;
in vec2 TextureCoords;

uniform sampler2D depthTexture;

float LinearizeDepth(float depth)
{   
   float far = 40.0;
   float near = 10.0;
   float z = depth * 2.0 - 1.0;
   return (2.0 * near) / (far + near - z * (far - near));
}

void main()
{

float depthValue = texture(depthTexture,TextureCoords).r;
if(depthValue == 1.0)
color = vec4(0.5,0.5,0.5,1.0);
else
{
//Orphographic
color = vec4 (vec3(depthValue),1.0f);
//Perspective
//color = vec4 (vec3(LinearizeDepth(depthValue)),1.0f);
}

}