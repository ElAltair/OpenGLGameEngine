#version 420 core

out vec4 color;
in vec3 normal;
in vec3 fragPosition;
uniform sampler2D TextureWall;
uniform sampler2D TextureSmile;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
void main( )
{
	float ambientStrength = 0.1f;
	float specularStrength = 0.5f;
	vec3 norm = normalize( normal );
	vec3 lightDir = normalize( lightPos - fragPosition );
	float diff = max( dot( norm, lightDir ), 0.0 );
	vec3 diffuse = diff * lightColor;
	vec3 ambient = lightColor * ambientStrength;
	vec3 ViewDir = normalize( viewPos - fragPosition );
	vec3 reflectDir = reflect( -lightDir, norm );
	float spec = pow( max( dot( ViewDir, reflectDir ), 0.0 ), 32 );
	vec3 specular = lightColor * spec * specularStrength;
	vec3 resultColor = (ambient + diffuse + specular) * objectColor;
	color = vec4( resultColor, 1.0f );

}