#ifndef _EXCEPTION_HPP_
#define _EXCEPTION_HPP_

#include <exception>
#include <string>

namespace ogls {

	class OglsException : public std::exception {
	public:
		OglsException(std::string err);
		virtual const char * what () const throw ();
   	private:
   		std::string m_err;
	};

	class ProgramException : public OglsException {
	public:
		ProgramException(std::string err) : OglsException(err) {}
	};

	class ShaderException : public OglsException {
	public:
		ShaderException(std::string err) : OglsException(err) {}
	};

}

#endif