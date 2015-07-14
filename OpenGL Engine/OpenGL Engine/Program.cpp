#include "Program.h"
#include "Shader.h"


Program::Program(const string &v,const  string &f) :VertexShaderName(v), FragmentShaderName(f)
{
	vertexShader = new Shader(VertexShaderName, GL_VERTEX_SHADER);
	vertexShader->loadSource();
	vertexShader->compileShader();
	fragmentShader = new Shader(FragmentShaderName, GL_FRAGMENT_SHADER);
	fragmentShader->loadSource();
	fragmentShader->compileShader();
}

Program::Program()
{

}

void Program::addShader(const string& shaderSource, int type)
{
	if (type == GL_VERTEX_SHADER)
	{
		vertexShader = new Shader(shaderSource, type);
		vertexShader->loadSource();
		vertexShader->compileShader();
	}
	else
	{

		fragmentShader = new Shader(shaderSource, type);
		fragmentShader->loadSource();
		fragmentShader->compileShader();
	}

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
		std::cout << "PROGRAM::" << ProgramName << "LINK ERROR\n" << ProgramInfoLog << std::endl;
		return false;
	}
	std::cout << "PROGRAM::" << ProgramName << "LINK COMPLETED \n";
	return true;
}

GLint Program::returnProgramName(void)
{
	return ProgramNameGL;
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
