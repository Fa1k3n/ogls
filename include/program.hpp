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
		Program& link();
		Program& use();
		Program& operator<<(Shader&);

		//! set a uniform value
		/*!	
		
			will throw exception if uniform with given name does not exists

		*/
		//!	\param name the name of the uniform
		//!	\param a value to set to
		//! \returns referens to object 
		template<typename T> Program& setUniform(const GLchar *name, T a);

		//! set a uniform2 value
		/*!	
		
			will throw exception if uniform with given name does not exists

		*/
		//!	\param name the name of the uniform
		//!	\param a, b value to set to
		//! \returns referens to object 
		template<typename T> Program& setUniform(const GLchar *name, T a, T b);

		//! set a uniform3 value
		/*!	
		
			will throw exception if uniform with given name does not exists

		*/
		//!	\param name the name of the uniform
		//!	\param a, b, c value to set to
		//! \returns referens to object 
		template<typename T> Program& setUniform(const GLchar *name, T a, T b, T c);		

		//! set a uniform4 value
		/*!	
		
			will throw exception if uniform with given name does not exists

		*/
		//!	\param name the name of the uniform
		//!	\param a, b, c, d value to set to
		//! \returns referens to object
		template<typename T> Program& setUniform(const GLchar *name, T a, T b, T c, T d);

	private:
		Shader* getShaderByType(GLenum type);
		void checkErrors();
		int glGetUniformLocationAndCheckError(const GLchar* name);
		std::vector<Shader*> m_shaders;
		GLuint m_id;
		bool m_isLinked;
	};
}

#endif