#version 450 core

out vec2 color;
in vec2 TexCoords;

uniform sampler2D depthMap;

void main()
{

float depthValue = texture(depthMap,Texcoords).r);
color = ve4(vec4(depthValue),1.0);

}