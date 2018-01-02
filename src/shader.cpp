
#include "shader.hpp"
#include "colors_priv.hpp"

#include <stdio.h>

using namespace ogls;

Shader::Shader(GLenum shaderType) {
	if (shaderType != GL_VERTEX_SHADER && shaderType != GL_FRAGMENT_SHADER) {
		throw ShaderException("Not a valid shader type");
	}
	m_type = shaderType;
	m_id = glCreateShader(m_type);
	if (m_id == 0)
		throw ShaderException("Error creating shader"); 
}

GLenum Shader::type() {
	return m_type;
}

Shader& Shader::addSource(std::istream &stream) {
	std::string s(std::istreambuf_iterator<char>(stream), {});
	return addSource(s);
}

Shader& Shader::addSource(std::string source) {
	m_sources.push_back(source.c_str());	
	transferSources();
	return *this;
}

Shader& Shader::compile() {
	glCompileShader(m_id);
	int  success;
	char infoLog[512];
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
	if(!success)
	{
	    glGetShaderInfoLog(m_id, 512, NULL, infoLog);
	    throw ShaderException(infoLog);
	} 
	return *this;
}

char* Shader::getVersionStr() {
	static char str[100];
	sprintf(str, "#version %d%d core", m_version[0], m_version[1]);
	return str;
}

void Shader::transferSources() {
	glShaderSource(m_id, m_sources.size(), &m_sources[0], NULL);
	GLenum err = glGetError();
	if(err != GL_NO_ERROR) {
		if(err == GL_INVALID_VALUE)
			throw ShaderException("[GL_INVALID_VALUE] not a valid shader or no sources set");
		else if(err == GL_INVALID_OPERATION)
			throw ShaderException("[GL_INVALID_OPERATION] not a valid shader object");
		else
			throw ShaderException("unknown error occurred");
	}
}

Shader& Shader::setVersion(short major, short minor) {
	m_version[0] = major;
	m_version[1] = minor;
	m_sources.insert(m_sources.begin(), getVersionStr());
	transferSources();
	return *this;
}

FragmentShader::FragmentShader(std::string source) : Shader(GL_FRAGMENT_SHADER) {
	addSource(source);
}

VertexShader::VertexShader(std::string source) : Shader(GL_VERTEX_SHADER) {
	addSource(source);
}

SolidColorShader::SolidColorShader(Color color) : SolidColorShader(color_presets[color]) {}

SolidColorShader::SolidColorShader(ColorRGB color) {
	m_color = color;
	char buff[100];
	snprintf(buff, sizeof(buff), "#version 330 core\n"
		"out vec4 FragColor;"
		"void main() {"
		"	FragColor = vec4(%.3ff, %.3ff, %.3ff, 1.0f);"
		"}", color.r, color.g, color.b);
	addSource(buff);
}