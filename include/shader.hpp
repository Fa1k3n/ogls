#ifndef _OGLS_SHADER_HPP_
#define _OGLS_SHADER_HPP_

#include <GL/glew.h>

#include <exception>
#include <string>
#include <istream>

#include <vector>

#include "colors.hpp"

namespace ogls {
	class Program;

	struct ShaderException : public std::exception {
		ShaderException(std::string err) {
			m_err = err;
		}
		const char * what () const throw () {
      		return m_err.c_str();
   		}
   		std::string m_err;
	};

	class Shader {
		friend Program;
	public:

		Shader(GLenum shaderType);
		GLenum type();
		Shader& addSource(std::istream &stream);
		Shader& addSource(std::string source);
		Shader& compile();
		Shader& setVersion(short major, short minor);

	private:
		GLenum m_type;
		GLint m_id;
		bool m_isCompiled;
		short m_version[2];
		char* getVersionStr();
		void transferSources();
		std::vector<const char*> m_sources;

	};

	class VertexShader : public Shader {
	public:
		VertexShader() : Shader(GL_VERTEX_SHADER) {}
		VertexShader(std::string source);
	};

	class FragmentShader : public Shader {
	public:
		FragmentShader() : Shader(GL_FRAGMENT_SHADER) {}
		FragmentShader(std::string source);
	};

	class SolidColorShader : public FragmentShader {
	public: 
		SolidColorShader(ColorRGB color);
		SolidColorShader(Color color);
		ColorRGB color() { return m_color; }
	private:
		ColorRGB m_color;
	};
}

#endif