#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normalCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 normal;
out vec3 fragPosition;

void main()
{
gl_Position = projection * view * model * vec4(position.x,position.y,position.z, 1.0);
normal = mat3(transpose(inverse(model))) * normalCoords;
fragPosition = vec3(model * vec4(position,1.0f));
}