#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normalCoords;
layout (location = 2) in vec2 TextureCoordinates;

out VShader_out
{
     vec3 Normal;
	 vec3 FragPos;
	 vec2 TextureCoord;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
gl_Position = projection * view * model * vec4(position.x,position.y,position.z, 1.0);
vs_out.Normal = mat3(transpose(inverse(model))) * normalCoords;
vs_out.FragPos = vec3(model * vec4(position,1.0f));
vs_out.TextureCoord = TextureCoordinates;
}