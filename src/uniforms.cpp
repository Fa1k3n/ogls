#include "program.hpp"

namespace ogls {
	// FLOAT
	template<>
	Program& Program::setUniform<float>(const GLchar *name, float a) {
		glUniform1f(glGetUniformLocationAndCheckError(name), a);
		return *this;
	}

	template<>
	Program& Program::setUniform<float>(const GLchar *name, float a, float b) {
		glUniform2f(glGetUniformLocationAndCheckError(name), a, b);
		return *this;
	}

	template<>
	Program& Program::setUniform<float>(const GLchar *name, float a, float b, float c) {
		glUniform3f(glGetUniformLocationAndCheckError(name), a, b, c);
		return *this;
	}

	template<>
	Program& Program::setUniform<float>(const GLchar *name, float a, float b, float c, float d) {
		glUniform4f(glGetUniformLocationAndCheckError(name), a, b, c, d);
		return *this;
	}

	// INT
	template<>
	Program& Program::setUniform<int>(const GLchar *name, int a) {
		glUniform1i(glGetUniformLocationAndCheckError(name), a);
		return *this;
	}

	template<>
	Program& Program::setUniform<int>(const GLchar *name, int a, int b) {
		glUniform2i(glGetUniformLocationAndCheckError(name), a, b);
		return *this;
	}

	template<>
	Program& Program::setUniform<int>(const GLchar *name, int a, int b, int c) {
		glUniform3i(glGetUniformLocationAndCheckError(name), a, b, c);
		return *this;
	}

	template<>
	Program& Program::setUniform<int>(const GLchar *name, int a, int b, int c, int d) {
		glUniform4i(glGetUniformLocationAndCheckError(name), a, b, c, d);
		return *this;
	}

	// UNSIGNED INT
	template<>
	Program& Program::setUniform<unsigned int>(const GLchar *name, unsigned int a) {
		glUniform1ui(glGetUniformLocationAndCheckError(name), a);
		return *this;
	}

	template<>
	Program& Program::setUniform<unsigned int>(const GLchar *name, unsigned int a, unsigned int b) {
		glUniform2ui(glGetUniformLocationAndCheckError(name), a, b);
		return *this;
	}

	template<>
	Program& Program::setUniform<unsigned int>(const GLchar *name, unsigned int a, unsigned int b, unsigned int c) {
		glUniform3ui(glGetUniformLocationAndCheckError(name), a, b, c);
		return *this;
	}

	template<>
	Program& Program::setUniform<unsigned int>(const GLchar *name, unsigned int a, unsigned int b, unsigned int c, unsigned int d) {
		glUniform4ui(glGetUniformLocationAndCheckError(name), a, b, c, d);
		return *this;
	}
}