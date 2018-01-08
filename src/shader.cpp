
#include "shader.hpp"
#include "colors_priv.hpp"

#include <stdio.h>
#include <iostream>

using namespace ogls;

//! creates a new shader object of a given type. 
/*!
	Valid shader types are GL_VERTEX_SHADER and GL_FRAGMENT_SHADER.

	If unsuccessful it will throw an ShaderException		

	\param shaderType the type of shader
	\sa VertexShader(std::string), FragmentShader(std::string)
*/
Shader::Shader(GLenum shaderType) {
	if (shaderType != GL_VERTEX_SHADER && shaderType != GL_FRAGMENT_SHADER) {
		throw ShaderException("Not a valid shader type");
	}
	m_type = shaderType;
	m_isCompiled = false;
	m_id = glCreateShader(m_type);
	if (m_id == 0)
		throw ShaderException("Error creating shader"); 
}

//! type is used to query the Shader of it's type
//! \returns shader type
GLenum Shader::type() {
	return m_type;
}

//! adds a istream source to the shader
/*!	the added block will be added last to the
	currently added code

	Example for an fstream
	\code{.cpp}
	std::fstream fragFile;
    fragFile.open("some file");
	auto shdr = Shader(FragmentShader).addSource(fragFile);
	fragFile.close();
	\endcode

	if unsuccessful will throw an ShaderException
*/
//! \param stream an istream to read from
//! \returns referens to object 
//! \sa addSource(std::string)
Shader& Shader::addSource(std::istream &stream) {
	std::string s(std::istreambuf_iterator<char>(stream), {});
	return addSource(s);
}

//! add a string source to the shader
/*!
	the added block will be added last to the
	currently added code

	Example
	\code{.cpp}
	auto shdr = Shader(FragmentShader).addSource(
		"int main(void) {"
		"	gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);"
		"}"
	);
	\endcode
	if unsuccessful will throw an ShaderExpection
*/ 
//! \param source is a string source
//! \returns referens to object
//! \sa addSource(std::istream)
Shader& Shader::addSource(std::string source) {
	m_sources.push_back(source);
	transferSources();
	m_isCompiled = false;
	return *this;
}

//! add a string source to the shader
/*!
	the added block will be added last to the
	currently added code

	Example
	\code{.cpp}
	auto shdr = Shader(FragmentShader) << "foo" << "bar";
	\endcode
	if unsuccessful will throw an ShaderExpection
*/ 
//! \param source is a string source
//! \returns referens to object
//! \sa addSource(std::istream)
Shader& Shader::operator<<(std::string source) {
	return addSource(source);
}

//! compiles the shader
/*!
	Íf the shader is already compiled this will silently return
	if unsuccessfull will throw and ShaderException
*/
//! \returns reference to object
Shader& Shader::compile() {
	if (m_sources.size() > 0 && !m_isCompiled) {
		glCompileShader(m_id);
		int  success;
		char infoLog[512];
		glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
		if(!success)
		{
		    glGetShaderInfoLog(m_id, 512, NULL, infoLog);
		    throw ShaderException(infoLog);
		} 
		m_isCompiled = true;
	}
	return *this;
}

//! sets the version header of the shader program
/*!
	Will set the version string of the shader program
	to
	#version <major><minor> core
	The code will be reuploaded to the device memory
	so a ShaderException can happen
*/
//! \param major is the major version
//! \param minor is the minor version
//! \returns reference to object
Shader& Shader::setVersion(short major, short minor) {
	m_version[0] = major;
	m_version[1] = minor;
	m_sources.insert(m_sources.begin(), getVersionStr());
	transferSources();
	return *this;
}

std::string Shader::getVersionStr() {
	static char str[150];
	sprintf(str, "#version %d%d core", m_version[0], m_version[1]);
	return std::string(str);
}

void Shader::transferSources() {
	const char** sources = (const char**)malloc(m_sources.size()*sizeof(char *));
	for (unsigned int i = 0; i < m_sources.size(); i ++) 
		sources[i] = m_sources[i].c_str();

	glShaderSource(m_id, m_sources.size(), &sources[0], NULL);
	free(sources);
	
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

//! creates a new fragment shader object with the given source
/*!
	If unsuccessful it will throw an ShaderException		

	\param source is the string source of the shader
*/
FragmentShader::FragmentShader(std::string source) : Shader(GL_FRAGMENT_SHADER) {
	addSource(source);
}

//! creates a new vertex shader object with the given source
/*!
	If unsuccessful it will throw an ShaderException		

	\param source is the string source of the shader
*/
VertexShader::VertexShader(std::string source) : Shader(GL_VERTEX_SHADER) {
	addSource(source);
}

//! creates a new solid shader object with the given color
/*!
	available colors are listed in colors.hpp

	\param source is the color enum
*/
SolidColorShader::SolidColorShader(Color color) : SolidColorShader(color_presets[color]) {}

//! creates a new solid shader object with the given RGB color
/*!
	\code{.cpp}
	auto shdr = SolidColorShader(ColorsRGB{1.0f, 0.0f, 0.0f});
	\endcode

	\param source is the RGB color 
*/
SolidColorShader::SolidColorShader(ColorRGB color) {
	m_color = color;
	char buff[100];
	snprintf(buff, sizeof(buff), "#version 330 core\n"
		"out vec4 FragColor;"
		"void main() {"
		"	FragColor = vec4(%.3ff, %.3ff, %.3ff, 1.0f);"
		"}", color.r, color.g, color.b);
	addSource(buff);
	compile();
}