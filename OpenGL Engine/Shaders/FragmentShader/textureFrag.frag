#version 450 core

in VShader_out
{
    vec3 Normal;
	vec3 FragPos;
	vec2 TextureCoord;
}fs_in;

out vec4 color;
uniform sampler2D Cubetexture;

void main()
{
color = texture(Cubetexture,fs_in.TextureCoord);
}