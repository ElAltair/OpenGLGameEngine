#include "Program.h"
#include "Shader.h"
#include "Light.h"
#include "Material.h"


Program::Program(const string &v,const  string &f) :VertexShaderName(v), FragmentShaderName(f)
{

}

bool Program::loadShaders()
{
	vertexShader = new Shader(VertexShaderName, GL_VERTEX_SHADER);
	fragmentShader = new Shader(FragmentShaderName, GL_FRAGMENT_SHADER);
	if (!vertexShader->loadSource())
		return false;
	if (!vertexShader->compileShader())
		return false;

		if (!fragmentShader->loadSource())
			return false;
		if (!fragmentShader->compileShader())
			return false;
		return true;
}

Program::~Program()
{
}

bool Program::linkProgram(void)
{
	ProgramNameGL = glCreateProgram();
	GLint vertexShaderNameGL = vertexShader->returnShaderName();
	GLint fragmentShaderNameGL = fragmentShader->returnShaderName();
	glAttachShader(ProgramNameGL, vertexShaderNameGL);
	glAttachShader(ProgramNameGL, fragmentShaderNameGL);
	glLinkProgram(ProgramNameGL);
	GLint success;
	glGetProgramiv(ProgramNameGL, GL_COMPILE_STATUS, &success);
	GLchar ProgramInfoLog[512];
	if (!success)
	{
		glGetProgramInfoLog(ProgramNameGL,512, NULL, ProgramInfoLog);
		std::cout << "\tERROR :: PROGRAM :: " << ProgramName << "LINKING FAILED\n" << ProgramInfoLog << std::endl << std::endl;
		return false;
	}
	std::cout << "PROGRAM :: " << ProgramName << " LINKING COMPLETED \n" << std::endl << std:: endl;
	vertexShader->deleteShader();
	delete vertexShader;
	fragmentShader->deleteShader();
	delete fragmentShader;
	return true;
}

GLint Program::returnProgramNameID(void)
{
	return ProgramNameGL;
}

string Program::returnProgramName(void)
{
	return ProgramName;
}


void Program::setProgramName(string name)
{
	ProgramName = name;
}

void Program::use(void)
{
	glUseProgram(ProgramNameGL);
}

GLuint Program::getLocation(const GLchar* name)
{
	GLuint location =  glGetUniformLocation(ProgramNameGL,name);
	return location;
}

void Program::setUniformData(const GLchar* name, GLfloat data)
{
	GLuint location = glGetUniformLocation(ProgramNameGL, name);
	glUniform1f(location, data);
}
void Program::setUniformData(const GLchar* name, GLfloat f1,GLfloat f2)
{
	GLuint location = glGetUniformLocation(ProgramNameGL, name);
	glUniform2f(location,f1,f2);
}
void Program::setUniformData(const GLchar* name, GLfloat f1,GLfloat f2, GLfloat f3)
{
	GLuint location = glGetUniformLocation(ProgramNameGL, name);
	glUniform3f(location,f1,f2,f3);
}
void Program::setUniformData(const GLchar* name, glm::mat4& matrix)
{
	GLuint location = glGetUniformLocation(ProgramNameGL, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

/*
void Program::setLight(Light& light)
{
	glm::vec3 ambientLight = light.getAmbient();
	glm::vec3 diffuseLight = light.getDiffuse();
	glm::vec3 specularLight = light.getSpecular();
	glm::vec3 lightPosition = light.getPostition();
	setUniformData("light.ambient", ambientLight.x, ambientLight.y, ambientLight.z);
	setUniformData("light.diffuse", diffuseLight.x, diffuseLight.y, diffuseLight.z);
	setUniformData("light.specular", specularLight.x, specularLight.y, specularLight.z);
	setUniformData("light.position", lightPosition.x, lightPosition.y, lightPosition.z);

}
*/
void Program::setLight(DirectionalLight& light)
{
	glm::vec3 ambientLight = light.getAmbient();
	glm::vec3 diffuseLight = light.getDiffuse();
	glm::vec3 specularLight = light.getSpecular();
	glm::vec3 lightDirection = light.getDirection();
	setUniformData("light.ambient", ambientLight.x, ambientLight.y, ambientLight.z);
	setUniformData("light.diffuse", diffuseLight.x, diffuseLight.y, diffuseLight.z);
	setUniformData("light.specular", specularLight.x, specularLight.y, specularLight.z);
	setUniformData("light.direction", lightDirection.x, lightDirection.y, lightDirection.z);

}
void Program::setMaterial(Material& material)
{
	glm::vec3 ambientLight = material.getAmbient();
	glm::vec3 diffuseLight = material.getDiffuse();
	glm::vec3 specularLight = material.getSpecular();
	setUniformData("material.ambient", ambientLight.x, ambientLight.y, ambientLight.z);
	setUniformData("material.diffuse", diffuseLight.x, diffuseLight.y, diffuseLight.z);
	setUniformData("material.specular", specularLight.x, specularLight.y, specularLight.z);
	setUniformData("material.shininess", material.getShininess());

}