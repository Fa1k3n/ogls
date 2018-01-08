#ifndef _OGLS_SHADER_HPP_
#define _OGLS_SHADER_HPP_

#include <GL/glew.h>

#include <string>
#include <istream>

#include <vector>

#include "exception.hpp"
#include "colors.hpp"

namespace ogls {
	class Program;

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

		virtual Shader& operator<<(std::string source);

	protected:
		virtual bool isCompiled() { return m_isCompiled; }
		virtual GLint id() { return m_id; }

	private:
		std::string getVersionStr();
		void transferSources();

		GLenum m_type;
		GLint m_id;
		bool m_isCompiled;
		short m_version[2];
		std::vector<std::string> m_sources;
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