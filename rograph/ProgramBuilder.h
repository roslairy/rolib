#pragma once
#include "GL\glew.h"
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;
class ProgramBuilder {
public:
	ProgramBuilder();
	~ProgramBuilder();
	// build a program
	GLuint build();
	GLboolean addShader(GLenum type, const char* file);
protected:
	vector<GLuint> _shaders;
};
