#ifndef PROGRAM_H
#define PROGRAM_H
#include <string>
#include <glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
using std::string;

class Shader;
class Material;
class Light;
class DirectionalLight;
class PointLight;
class Program
{
public:
	Program(const string& v,const string&f);
	Program( const string& g );
	bool loadShaders();
	
	GLint returnProgramNameID(void);
	string returnProgramName(void);
	inline Shader* returnVertexShader( void ) { return vertexShader; };
	inline Shader* returnFragmentShader( void ) { return fragmentShader; };
	void setProgramName(string name);
	bool linkProgram(void);
	void use(void);

	GLuint getLocation(const GLchar* name);
	void setUniformData(const GLchar* name,GLfloat data);
	void setUniformData(const GLchar* name,GLfloat f1,GLfloat f2);
	void setUniformData(const GLchar* name,GLfloat f1, GLfloat f2, GLfloat f3);
	void setUniformData(const GLchar* name, glm::mat4& matrix);
	void setMaterial(Material& material);
	void setLight(DirectionalLight& light);
	void setLight(Light& light);
	void setLight(PointLight& light);


	int place;


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

