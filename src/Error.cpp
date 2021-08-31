/*
 * Error.cpp
 *
 *  Created on: 28 de mai. de 2021
 *      Author: thiago
 */

#include <fprint_abstraction/exceptions/Error.h>


#include <fprint_abstraction/utils.h>


Error::Error(const char* msg)
{
	this->msg = utils::copy_str(msg);
}

Error::Error(const std::string& msg)
{
	this->msg = utils::copy_str(msg);
}


Error::Error(const Error& error)
{
	msg = utils::copy_str(error.msg);
}


Error::Error(Error&& error)
{
	msg = utils::copy_str(error.msg);
	error.msg = nullptr;
}


Error& Error::operator=(Error& error)
{
	msg = utils::copy_str(error.msg);
	return *this;
}


Error& Error::operator=(Error&& error)
{
	msg = utils::copy_str(error.msg);
	error.msg = nullptr;

	return *this;
}


const char* Error::what() const noexcept
{
	return msg;
}


Error::~Error()
{
	if (msg) {
		delete [] msg;
	}
}
