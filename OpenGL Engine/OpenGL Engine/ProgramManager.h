#ifndef PROGRAM_MANAGER_H
#define PROGRAM_MANAGER_H

#include <string>
#include <iostream>
#include <vector>
#include <glew.h>
using std::string;

class Program;

class ProgramManager
{
public:
	ProgramManager();
	bool readConfig(void);
	bool createPrograms(void);
	GLint returnProgramId(string name);
	Program* returnProgram(string name);
	~ProgramManager();
private:
	string ConfigPath;
	int programCount;
	std::vector<string> shaderNames;
	std::vector<string> programNames;
	std::vector<Program*> programs;
};

#endif