#ifndef SHADER_H
#define SHADER_H
#include<string>
#include<fstream>
#include<iostream>
#include<sstream>
#include"glew.h"
using std::string;

class Shader
{
public:
	Shader(const string& str, int type);
	bool loadSource(void);
	bool compileShader(void);
	GLint returnShaderName(void);
	void deleteShader(void);
	//void attachShader(Program* pr);


private:
	string ShaderName;
	GLuint ShaderNameID;
	string source;
	int ShaderType;
};
#endif