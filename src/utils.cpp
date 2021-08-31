/*
 * utils.cpp
 *
 *  Created on: 23 de abr. de 2021
 *      Author: thiago
 */

#include <fprint_abstraction/utils.h>


namespace utils
{
	gchar* string_to_char(std::string &src)
	{
		gsize length = src.length() + 1;
		gchar *c_string = g_new0(gchar, length);
		return std::strcpy(c_string, src.c_str());
	}


	void string_to_char(gchar *buffer, gsize buffer_size, std::string &src)
	{
		gsize length = std::min(buffer_size, src.size());
		std::strncpy(buffer, src.c_str(), length);
	}


	char* copy_str(const std::string& str)
	{
		char* new_str = new char[str.size()+1];
		std::strcpy(new_str, str.c_str());
		return new_str;
	}


	char* copy_str(const char* str)
	{
		size_t size = std::strlen(str);
		char* new_str = new char[size+1];
		std::strcpy(new_str, str);
		return new_str;
	}


	extern "C"
	{
		GDate* new_gdate(int day, int month, int year)
		{
			return g_date_new_dmy((GDateDay) day,
					              (GDateMonth) month,
								  (GDateYear) year);
		}

		GDate* new_gdate_now()
		{
			g_autoptr(GDateTime) datetime = g_date_time_new_now_local();
			gint year, month, day;
			g_date_time_get_ymd(datetime, &year, &month, &day);
			return g_date_new_dmy((GDateDay) day,
					              (GDateMonth) month,
								  (GDateYear) year);
		}

		utils::simple_date gdate_to_simple_date(const GDate *gdate)
		{
			int year = g_date_get_year(gdate);
			int month = g_date_get_month(gdate);
			int day = g_date_get_day(gdate);

			return { year, month, day };
		}
	};


	std::ostream& operator<<(std::ostream& stream, const utils::simple_date&& date)
	{
		std::stringstream ss;
		ss << date.day << "/" << date.month << "/" << date.year;
		return stream << ss.str();
	}
};
