#include "program.hpp"

#include <iostream>

using namespace ogls;

Program::Program() {
	m_id = glCreateProgram();
	m_isLinked = false;
	if (m_id == 0)
		throw ProgramException("Failed to create program");
}		

Program& Program::addShader(Shader &shader) {
	if(getShaderByType(shader.type())) {
		throw ProgramException("Only one shader of a type is allowed");
	}
	if(!shader.isCompiled()) {
		shader.compile();
	}
	m_shaders.push_back(&shader);
	glAttachShader(m_id, shader.id());
	GLenum err = glGetError();
	if(err != GL_NO_ERROR) {
		if(err == GL_INVALID_VALUE)
			throw ProgramException("[GL_INVALID_VALUE] not a valid program");
		else if(err == GL_INVALID_OPERATION)
			throw ProgramException("[GL_INVALID_OPERATION] already attached?");
		else
			throw ProgramException("unknown error occurred");
	}
	m_isLinked = false;
	return *this;
}

Shader* Program::getShaderByType(GLenum type) {
	int i = 0;
	for(auto shader : m_shaders) {
		//std::cout << "Comparing type: " << type << " " << shader.type() << std::endl;
		if (shader->type() == type) {
			return m_shaders[i];
		}
		i ++;
	}
	return NULL;
}

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

Program& Program::use() {
	if(!m_isLinked)
		link();
	glUseProgram(m_id);
	GLenum err = glGetError();
	if(err != GL_NO_ERROR) {
		if(err == GL_INVALID_VALUE)
			throw ProgramException("[GL_INVALID_VALUE] not a valid program");
		else if(err == GL_INVALID_OPERATION)
			throw ProgramException("[GL_INVALID_OPERATION] could not be made part of current state");
		else
			throw ProgramException("unknown error occurred");
	}
	return *this;
}		

