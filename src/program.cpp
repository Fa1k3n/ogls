#include "program.hpp"

#include <iostream>

using namespace ogls;

//! creates a new program object 
/*!
	If unsuccessful it will throw an ProgramException		
*/
Program::Program() {
	m_id = glCreateProgram();
	m_isLinked = false;
	if (m_id == 0)
		throw ProgramException("Failed to create program");
}		

//! adds a shader to the program
/*!	the added shader will be added to the list of shaders

	If the shader is not compiled then will try to compile 
	shader first 

	if there already is a shader of a given type added this 
	will throw an exception

	Example 
	\code{.cpp}
	ogls::Program p;
	p.addShader(ogls::SolidColorShader(ogls::RED)):
	\endcode

*/
//!	\param shader the shader to attach
//! \returns referens to object 
//! \sa operator<<(Shader& shdr)
Program& Program::addShader(Shader &shader) {
	if(getShaderByType(shader.type())) {
		throw ProgramException("Only one shader of a type is allowed");
	}
	if(!shader.isCompiled()) {
		shader.compile();
	}
	m_shaders.push_back(&shader);
	glAttachShader(m_id, shader.id());
	checkErrors();
	m_isLinked = false;
	return *this;
}

//! adds shaders to the program
/*!	the added shaders will be added to the list of shaders

	if there already is a shader of a given type added this 
	will throw an exception

	Example 
	\code{.cpp}
	ogls::Program p;
	p << ogls::SolidColorShader(ogls::RED):
	\endcode

*/
//! \returns referens to object 
//! \sa addShader(Shader &shader)
Program& Program::operator<<(Shader& shdr) {
	addShader(shdr);
	return *this;
}

//! links all the attached shaders
/*!
	Will throw program exception if something when wrong
*/
//! \returns regerence to self
Program& Program::link() {
	if(m_shaders.size() > 0 && !m_isLinked) {
		glLinkProgram(m_id);
		int  success;
		char infoLog[512];
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if(!success) {
	    	glGetProgramInfoLog(m_id, 512, NULL, infoLog);
	    	throw ProgramException(infoLog);
		}
		m_isLinked = true;
	}
	return *this;
}

//! use the program
/*!
	If program is not linked it will try to link it first

	Will throw program exception if something when wrong
*/
//! \returns regerence to self
Program& Program::use() {
	if(!m_isLinked)
		link();
	glUseProgram(m_id);
	checkErrors();
	return *this;
}		

Shader* Program::getShaderByType(GLenum type) {
	int i = 0;
	for(auto shader : m_shaders) {
		if (shader->type() == type) {
			return m_shaders[i];
		}
		i ++;
	}
	return NULL;
}

void Program::checkErrors() {
	GLenum err = glGetError();
	if(err != GL_NO_ERROR) {
		if(err == GL_INVALID_VALUE)
			throw ProgramException("[GL_INVALID_VALUE] not a valid program");
		else if(err == GL_INVALID_OPERATION)
			throw ProgramException("[GL_INVALID_OPERATION] could not be made part of current state");
		else
			throw ProgramException("unknown error occurred");
	}
}