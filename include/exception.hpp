#ifndef _EXCEPTION_HPP_
#define _EXCEPTION_HPP_

#include <exception>
#include <string>

namespace ogls {

	//! base class for any exception
	class OglsException : public std::exception {
	public:
		//! create a base exception, should never be used 
		OglsException(std::string err);
		//! returns the error message
		//! \returns error message
		virtual const char * what () const throw ();
   	private:
   		std::string m_err;
	};

	//! base class for program exception
	class ProgramException : public OglsException {
	public:
		//! create a new program exception
		//! \param err exception string
		ProgramException(std::string err) : OglsException(err) {}
	};

	//! base class for shader exception
	class ShaderException : public OglsException {
	public:
		//! create a new shader exception
		//! \param err exception string
		ShaderException(std::string err) : OglsException(err) {}
	};

}

#endif