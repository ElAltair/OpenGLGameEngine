#ifndef PROGRAM_H
#define PROGRAM_H
#include <string>
#include <glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
using std::string;

class Shader;
class Program
{
public:
	Program(const string& v,const string&f);
	bool loadShaders();
	
	GLint returnProgramNameID(void);
	string returnProgramName(void);
	void setProgramName(string name);
	bool linkProgram(void);
	void use(void);

	GLuint getLocation(const GLchar* name);
	void setUniformData(const GLchar* name,GLfloat data);
	void setUniformData(const GLchar* name,GLfloat f1,GLfloat f2);
	void setUniformData(const GLchar* name,GLfloat f1, GLfloat f2, GLfloat f3);
	void setUniformData(const GLchar* name, glm::mat4& matrix);
	

	~Program();
private:
	string ProgramName;
	GLuint ProgramNameGL;
	Shader* vertexShader;
	Shader* fragmentShader;
	string VertexShaderName;
	string FragmentShaderName;
};
#endif

