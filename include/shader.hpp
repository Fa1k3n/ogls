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
		virtual const char * what () const throw () {
      		return m_err.c_str();
   		}
   		std::string m_err;
	};

	class Shader {
		friend Program;
	public:
		Shader() {}  // This is for testability
		Shader(GLenum shaderType);

		virtual ~Shader() {}
		virtual GLenum type();
		virtual Shader& addSource(std::istream &stream);
		virtual Shader& addSource(std::string source);
		virtual Shader& compile();
		virtual Shader& setVersion(short major, short minor);

	protected:
		virtual bool isCompiled() { return m_isCompiled; }
		virtual GLint id() { return m_id; }

	private:
		char* getVersionStr();
		void transferSources();

		GLenum m_type;
		GLint m_id;
		bool m_isCompiled;
		short m_version[2];
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