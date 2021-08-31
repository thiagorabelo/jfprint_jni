/*
 * Error.h
 *
 *  Created on: 28 de mai. de 2021
 *      Author: thiago
 */

#ifndef INCLUDE_FPRINT_ABSTRACTION_EXCEPTIONS_ERROR_H_
#define INCLUDE_FPRINT_ABSTRACTION_EXCEPTIONS_ERROR_H_

#include<exception>
#include<string>

class Error : public std::exception
{
	public:
		Error(const char *msg);
		Error(const std::string& msg);

		Error(const Error& error);
		Error(Error&& error);

		Error& operator=(Error& error);
		Error& operator=(Error&& error);

		virtual const char* what() const noexcept;

		virtual ~Error();

	private:
		const char *msg;
};

#endif /* INCLUDE_FPRINT_ABSTRACTION_EXCEPTIONS_ERROR_H_ */
