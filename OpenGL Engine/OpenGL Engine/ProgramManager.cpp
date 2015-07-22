#include "ProgramManager.h"
#include "Program.h"
#include <fstream>


ProgramManager::ProgramManager()
{
	programCount = 0;
	ConfigPath= "../Shaders/ProgramManager.conf";
}


ProgramManager::~ProgramManager()
{
	std::cout << " Destruction \n" << std::endl;
	for (int i = 0; i < programs.size(); ++i)
	{
		delete programs.at(i);
	}
}

bool ProgramManager::readConfig(void)
{
	std::ifstream configFile;
	configFile.open(ConfigPath);
	if (!configFile.is_open())
	{
		std::cout << "\tERROR :: PROGRAM MANAGER :: CONFIG FILE OPENNING FAILED " << std::endl << std::endl;
		return false;
	}
	else
	{
		std::cout << "PROGRAM MANAGER :: CONFIG FILE OPENNING COMPETED " << std::endl << std::endl;
	}
 
	std::string tmp;
	int i = 0;
	while (!configFile.eof())
	{
		configFile >> tmp;
			if (i == 0)
			{
				programNames.push_back(tmp);
				++i;
			}
			else if (i == 1 || i == 2)
			{
				shaderNames.push_back(tmp);
				if (i == 2)
					i = 0;
				else
				++i;

			}
	}
	programCount = programNames.size();
	return true;
}

bool ProgramManager::createPrograms(void)
{
	// 0 prog - 0 and 1 shader
	// 1 prog   2 and 3 shader
	// 2 prog   4 and 5 shader
	// 3 prog   6 and 7 shader
	// i - prog count = 0 ....;
	// shader 1 = i * 2;
	// shader 2 = i * 2 + 1;
	Program* newProg = 0;
	for (int i = 0; i < programCount; ++i)
	{
		newProg = new Program(shaderNames.at( 2 * i ), shaderNames.at(i * 2 + 1));
		newProg->setProgramName(programNames.at(i));
		if (!newProg->loadShaders())
			return false;
		if (!newProg->linkProgram())
		{
			return false;
		}
		programs.push_back(newProg);

	}

	return true;
}

GLint ProgramManager::returnProgramId(string name)
{
	for (int i = 0; i < programs.size(); ++i)
	{
		if (name == programs.at(i)->returnProgramName())
			return programs.at(i)->returnProgramNameID();
	}
	return 0;
}

Program* ProgramManager::returnProgram(string name)
{
	for (int i = 0; i < programs.size(); ++i)
	{
		if (name == programs.at(i)->returnProgramName())
			return programs.at(i);
	}
	return 0;
}

