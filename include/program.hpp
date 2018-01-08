#ifndef _PROGRAM_HPP_
#define _PROGRAM_HPP_

#include <GL/glew.h>

#include <exception>
#include <string>
#include <vector>
#include <initializer_list>

#include "shader.hpp"
#include "exception.hpp"

namespace ogls {
	class Program {
	public:
		Program();
		Program& addShader(Shader &shader);
		template <typename T> 
		Program& setUniform(const GLchar *name, std::initializer_list<T> v);

		Program& link();
		Program& use();
		Program& operator<<(Shader&);

	private:
		Shader* getShaderByType(GLenum type);
		void checkErrors();

		std::vector<Shader*> m_shaders;
		GLuint m_id;
		bool m_isLinked;
	};

	//! set a uniform value
	/*!	
		
		will throw exception if uniform with given name does not exists

		Example 
		\code{.cpp}
		p.setUniform("my1dUni", {1.0f}).setUniform("my3dUni", {1.0f, 1.0f, 1.0f})
		\endcode
	*/
	//!	\param name the name of the uniform
	//!	\param v an initializer list to initialize the value to
	//! \returns referens to object 
	template <typename T>
	Program& Program::setUniform(const GLchar *name, std::initializer_list<T> v) {
		int loc = glGetUniformLocation(m_id, name);
		if (loc == -1) 
			throw ProgramException("Failed to get uniform location");
		checkErrors();

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