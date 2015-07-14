#include"Shader.h"

Shader::Shader(const string& str,int type) : ShaderName(str)
{
	ShaderType = type;

}
bool Shader::loadSource(void)
{
	std::ifstream file;
	string filePath;

	if (ShaderType == GL_VERTEX_SHADER)
	filePath = "../Shaders/VertexShader/" + ShaderName;
	else if (ShaderType == GL_FRAGMENT_SHADER)
	filePath = "../Shaders/FragmentShader/" + ShaderName;

	file.open(filePath);
	
	if (file.is_open())
	{
		std::cout << "SHADER :: " << ShaderName << " :: SOURCE LOADING COMPLETE" << std::endl << std::endl;
	}
	else
	{
		std::cout << "\tERROR :: SHADER :: " << ShaderName << ":: SOURCE LOADING FAILED" << std::endl << std::endl;
		return false;
	}
	
	std::stringstream ShaderStream;
	ShaderStream << file.rdbuf();
	source = ShaderStream.str();
	file.close();
	return true;
}

bool Shader::compileShader(void)
{
	const GLchar* ShaderSourceGL = source.c_str();
	ShaderNameID = glCreateShader(ShaderType);
	glShaderSource(ShaderNameID, 1, &ShaderSourceGL, NULL);
	glCompileShader(ShaderNameID);
	
	GLint success;
	glGetShaderiv(ShaderNameID, GL_COMPILE_STATUS, &success);
	GLchar infoLog[512];
	if (!success)
	{
		glGetShaderInfoLog(ShaderNameID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER:: " << ShaderName << " COMPILATION_FAILED\n " << infoLog << std::endl << std::endl;
		return false;
	}
	else
		std::cout << "SHADER:: " << ShaderName << " ::COMPILATION_COMPLITED\n" << std::endl << std::endl;
	return true;
}

GLint Shader::returnShaderName(void)
{
	return ShaderNameID;
}

void Shader::deleteShader(void)
{
	glDeleteShader(ShaderNameID);
}
