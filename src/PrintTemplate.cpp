/*
 * PrintTemplate.cpp
 *
 *  Created on: 19 de mai. de 2021
 *      Author: thiago
 */

#include <fprint_abstraction/PrintTemplate.h>

#include <fprint_abstraction/Print.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <utility>


extern "C"
{
#include <glib-unix.h>
};


PrintTemplate::PrintTemplate() :
    finger(FP_FINGER_UNKNOWN),
	username(""),
	description(""),
	enroll_date{0,0,0}
{
	// TODO Auto-generated constructor stub

}


PrintTemplate::PrintTemplate(PrintTemplate& tmplt) :
	finger(tmplt.finger),
	username(tmplt.username),
	description(tmplt.description),
	enroll_date{tmplt.enroll_date.year, tmplt.enroll_date.month, tmplt.enroll_date.day}
{
	debug("Copying PrintTemplate");
}


PrintTemplate::PrintTemplate(PrintTemplate&& tmplt) :
	finger(tmplt.finger),
	username(std::move(tmplt.username)),
	description(std::move(tmplt.description)),
	enroll_date{tmplt.enroll_date.year, tmplt.enroll_date.month, tmplt.enroll_date.day}
{
	debug("Moving PrintTemplate");
}


PrintTemplate::~PrintTemplate()
{
	// TODO Auto-generated destructor stub
}


void PrintTemplate::set_finger(FpFinger finger)
{
	this->finger = finger;
}


FpFinger PrintTemplate::get_finger()
{
	return finger;
}


std::string& PrintTemplate::get_username()
{
	return username;
}


void PrintTemplate::set_username(std::string username)
{
	this->username = username;
}


std::string& PrintTemplate::get_description()
{
	return description;
}


void PrintTemplate::set_description(std::string description)
{
	this->description = description;
}


void PrintTemplate::set_enroll_date_now()
{
	g_autoptr(GDate) date = utils::new_gdate_now();
	enroll_date = utils::gdate_to_simple_date(date);
}


void PrintTemplate::set_enroll_date(int day, int month, int year)
{
	g_autoptr(GDate) date = utils::new_gdate(day, month, year);
	enroll_date = utils::gdate_to_simple_date(date);
}


utils::simple_date& PrintTemplate::get_enroll_date()
{
	return enroll_date;
}
