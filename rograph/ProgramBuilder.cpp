#include "ProgramBuilder.h"

ProgramBuilder::ProgramBuilder() {
	this->_shaders = vector<GLuint>();
}


ProgramBuilder::~ProgramBuilder() {
	int i = -1;
	while (++i < this->_shaders.size()) {
		glDeleteShader(this->_shaders[i]);
	}
}


// build a program
GLuint ProgramBuilder::build() {
	GLuint program = glCreateProgram();
	for (size_t i = 0; i < this->_shaders.size(); i++) {
		glAttachShader(program, this->_shaders[i]);
	}
	glLinkProgram(program);
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	return status == GL_TRUE ? GLuint(program) : 0;
}


GLboolean ProgramBuilder::addShader(GLenum type, const char* file) {
	ifstream fs(file);
	stringstream ss;
	if (!fs) return GL_FALSE;
	ss << fs.rdbuf();
	string shrStr(ss.str());
	fs.close();

	GLuint shader = glCreateShader(type);
	const char* vs1 = shrStr.c_str();
	glShaderSource(shader, 1, &vs1, NULL);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE) {
		this->_shaders.push_back(shader);
	}
	else {
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		fprintf(stdout, "Compile failure in a shader:\n%s\n", strInfoLog);
		delete[] strInfoLog;
	}
	return GLboolean(status);
}