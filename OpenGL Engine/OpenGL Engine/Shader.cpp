#include"Shader.h"

Shader::Shader(const string& str,int type) : ShaderName(str)
{
	ShaderType = type;

}
bool Shader::loadSource(void)
{
	std::ifstream file;
	string filePath = "../Shaders/" + ShaderName;
	file.open(filePath);
	
	if (file.is_open())
	{
		std::cout << "File is opened" << std::endl;
	}
	else
	{
		std::cout << "Error while openning shader source" << std::endl;
		return false;
	}
	
	std::cout << "Name = " << ShaderName << std::endl;
	std::stringstream ShaderStream;
	ShaderStream << file.rdbuf();
	source = ShaderStream.str();
	file.close();
}

bool Shader::compileShader(void)
{
	const GLchar* ShaderSourceGL = source.c_str();
	ShaderNameGL = glCreateShader(ShaderType);
	glShaderSource(ShaderNameGL, 1, &ShaderSourceGL, NULL);
	glCompileShader(ShaderNameGL);
	
	GLint success;
	glGetShaderiv(ShaderNameGL, GL_COMPILE_STATUS, &success);
	GLchar infoLog[512];
	if (!success)
	{
		glGetShaderInfoLog(ShaderNameGL, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << ShaderName<< "COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	else
		std::cout << "SHADER::" << ShaderName << "::COMPILATION_COMPLITED\n";
	return true;
}

GLint Shader::returnShaderName(void)
{
	return ShaderNameGL;
}

/*
void Shader::attachShader(Program* pr)
{
	glAttachShader(pr->returnProgramName(), ShaderNameGL);
}
*/