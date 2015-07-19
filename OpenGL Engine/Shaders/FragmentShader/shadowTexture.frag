#version 450 core


out vec4 color;
in vec2 TextureCoords;

uniform sampler2D depthTexture;
void main()
{
float depthValue = texture(depthTexture,TextureCoords).r;
color = vec4 ( vec3(depthValue),1.0f);
//color = vec4(1.0f,0.0f,0.0f,0.0f);
//color = vec4(TextureCoords.x,TextureCoords.y,0.0,1.0);
}