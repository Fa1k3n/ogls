#ifndef _PROGRAM_HPP_
#define _PROGRAM_HPP_

#include <GL/glew.h>

#include <exception>
#include <string>
#include <vector>
#include <initializer_list>

#include "shader.hpp"

namespace ogls {
	struct ProgramException : public std::exception {
		ProgramException(std::string err) {
			m_err = err;
		}
		const char * what () const throw () {
      		return m_err.c_str();
   		}
   		std::string m_err;
	};

	class Program {
	public:
		Program();
		Program& addShader(Shader &shader);
		template <typename T> 
		Program& setUniform(const GLchar *name, std::initializer_list<T> v);

		Program& link();
		Program& use();
	private:
		std::vector<Shader> m_shaders;
		Shader* getShaderByType(GLenum type);

		GLuint m_id;
	};

	template <typename T>
	Program& Program::setUniform(const GLchar *name, std::initializer_list<T> v) {
		int loc = glGetUniformLocation(m_id, name);
		std::vector<T> values(v);
	
		switch (values.size()) {
		case 1:
			glUniform1f(loc, values[0]);
			break;
		case 2:
			glUniform2f(loc, values[0], values[1]);
			break;	
		case 3:
			glUniform3f(loc, values[0], values[1], values[2]);
			break;
		case 4:
			glUniform4f(loc, values[0], values[1], values[2], values[3]);
			break;
		}

		return *this;
	}
}

#endif