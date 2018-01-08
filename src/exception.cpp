#include "exception.hpp"

using namespace ogls;

OglsException::OglsException(std::string err) {
	m_err = err;
}
		
const char * OglsException::what () const throw () {
	return m_err.c_str();
}
