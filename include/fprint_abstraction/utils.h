/*
 * utils.h
 *
 *  Created on: 22 de abr. de 2021
 *      Author: thiago
 */

#ifndef INCLUDE_FPRINT_ABSTRACTION_UTILS_H_
#define INCLUDE_FPRINT_ABSTRACTION_UTILS_H_


#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <cstring>
#include <cstdlib>


extern "C"
{
#include <glib-unix.h>
#include <libfprint-2/fprint.h>
};


/**
 * Cleanup functions generated by glib "G_DEFINE_AUTOPTR_CLEANUP_FUNC" macro.
 * 	/usr/include/glib-2.0/glib/gmacros.h
 */

// /usr/include/glib-2.0/glib/glib-autocleanups.h
#define RELEASE_GERROR(obj) glib_autoptr_cleanup_GError(&obj);

// /usr/include/libfprint-2/fp-context.h
#define RELEASE_FPPRINT(obj) glib_autoptr_cleanup_FpPrint(&obj)
#define RELEASE_FPCONTEXT(obj) glib_autoptr_cleanup_FpContext(&obj)
#define RELEASE_FPDEVICE(obj) glib_autoptr_cleanup_FpDevice(&obj)


/**
 * Log functions
 */

namespace
{
	template<typename T>
	void __log(std::stringstream& stream, T const& t)
	{
		stream << t << std::endl;
	}


	template<typename T, typename...Ts>
	void __log(std::stringstream& stream, T const& t, Ts const&... ts)
	{
		stream << t;
		__log(stream, ts...);
	}
}


template<typename T, typename L, typename F, typename... Ts>
void log(std::ostream &output, T type, L location, F func, Ts const&... ts)
{
	time_t now = time(0);
	struct tm tstruct;
	const size_t buffer_size = 80;
	char time_buffer[buffer_size];

	// Don't need cleanup: http://www.cplusplus.com/reference/clibrary/ctime/localtime/
	// It's not thread safe, making a copy.
	tstruct = *localtime(&now);

	// http://en.cppreference.com/w/cpp/chrono/c/strftime
	strftime(time_buffer, sizeof(char) * buffer_size, "%Y-%m-%d %X", &tstruct);

	std::string formated_time(time_buffer);

	std::stringstream stream;
	stream << type << " [" << formated_time << " " << location << " " << func << "] ";

	__log(stream, ts...);
	output << stream.str();
};


#define __TOSTR(i) #i
#define  _TOSTR(i) __TOSTR(i)
#define LOCATION_INFO __FILE__ ":" _TOSTR(__LINE__)
#define FUNC_DESC __PRETTY_FUNCTION__

#define LOG_STREAM std::cout


#ifndef NDEBUG
#define debug(head, ...) log(LOG_STREAM, "DEBUG", LOCATION_INFO, FUNC_DESC, head, ##__VA_ARGS__)
#else
#define debug(head, ...)
#endif


namespace utils
{
	gchar* string_to_char(std::string &src);

	void string_to_char(gchar *buffer, gsize buffer_size, std::string &src);

	char* copy_str(const std::string& str);

	char* copy_str(const char* str);

	extern "C"
	{
		GDate* new_gdate(int day, int month, int year);
		GDate* new_gdate_now();

		struct simple_date
		{
			int year;
			int month;
			int day;
		};

		utils::simple_date gdate_to_simple_date(const GDate *gdate);

		struct serialized_print
		{
			std::stringstream data;  // maybe change me to unique pointer?
			size_t size;
		};
	};

	std::ostream& operator<<(std::ostream& stream, const utils::simple_date&& date);
};


#endif /* INCLUDE_FPRINT_ABSTRACTION_UTILS_H_ */
