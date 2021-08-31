/*
 * Print.cpp
 *
 *  Created on: 22 de abr. de 2021
 *      Author: thiago
 */

#include <fprint_abstraction/Print.h>
#include <fprint_abstraction/exceptions/Error.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>


extern "C"
{
#include <glib-unix.h>
};


Print::Print(Print& other)
{
	debug("Copying Print [", other.print_ptr, "]");
	print_ptr = other.print_ptr;
}


Print::Print(Print&& other)
{
	debug("Moving Print [", other.print_ptr, "]");
	print_ptr = other.print_ptr;
	other.print_ptr = nullptr;
}


Print& Print::operator=(Print&& other)
{
	debug("Assign moving Print [", other.print_ptr, "]");
	print_ptr = other.print_ptr;
	other.print_ptr = nullptr;
	return *this;
}


Print::Print(FpDevice *device)
{
	debug("Create Print from FpDevice pointer [", device, "]");
	print_ptr = fp_print_new(device);
}


Print::Print(FpPrint *print)
{
	debug("Creating Print from FpPrint pointer [", print, "]");
	print_ptr = print;
}


Print::~Print()
{
	debug("Cleanup print [", print_ptr, "]");
	RELEASE_FPPRINT(print_ptr);
	print_ptr = nullptr;
}


std::string Print::get_driver()
{
	return fp_print_get_driver(print_ptr);
}


std::string Print::get_device_id()
{
	return fp_print_get_device_id(print_ptr);
}


FpFinger Print::get_finger()
{
	return fp_print_get_finger(print_ptr);
}


void Print::set_finger(FpFinger finger)
{
	fp_print_set_finger(print_ptr, finger);
}


std::string Print::get_username()
{
	return fp_print_get_username(print_ptr);
}


void Print::set_username(std::string& username)
{
	g_autofree gchar *c_string = utils::string_to_char(username);
	fp_print_set_username(print_ptr, c_string);
}


std::string Print::get_description()
{
	return fp_print_get_description(print_ptr);
}


void Print::set_description(std::string &description)
{
	g_autofree char *c_string = utils::string_to_char(description);
	fp_print_set_description(print_ptr, c_string);
}


void Print::set_enroll_date_now()
{
	g_autoptr(GDate) date = utils::new_gdate_now();
	fp_print_set_enroll_date(print_ptr, date);
}


void Print::set_enroll_date(int day, int month, int year)
{
	g_autoptr(GDate) date = utils::new_gdate(day, month, year);
	fp_print_set_enroll_date(print_ptr, date);
}


utils::simple_date Print::get_enroll_date()
{
	const GDate *gdate = fp_print_get_enroll_date(print_ptr); // transfer none

	if (!gdate) {
		return {0, 0, 0};
	}

	return utils::gdate_to_simple_date(gdate);
}


bool Print::equal(Print& other)
{
	return static_cast<bool>(fp_print_equal(print_ptr, other.print_ptr));
}


utils::serialized_print Print::serialize()
{
	gsize length = 0;
	g_autofree guchar *buffer = nullptr;
	g_autoptr(GError) error = nullptr;

	gboolean worked = fp_print_serialize(print_ptr, &buffer, &length, &error);

	if (!worked) {
		debug(error->message);
		throw Error(error->message);
	}

	utils::serialized_print serialized{
		std::stringstream(),
		0
	};

	serialized.data.write(reinterpret_cast<const char*>(buffer), sizeof(guchar)*length);
	serialized.size = static_cast<size_t>(length);

	return serialized;
}


Print Print::deserialize(utils::serialized_print& stream)
{
	gsize length = static_cast<gsize>(stream.size);
	g_autofree guchar *buffer = g_new0(guchar, length);
	g_autoptr(GError) error = nullptr;

	stream.data.read(reinterpret_cast<char*>(buffer), sizeof(guchar)*length);

	FpPrint *print = fp_print_deserialize(buffer, sizeof(guchar)*length, &error);

	if (error) {
		debug(error->message);
		throw Error(error->message);
	}

	return print;
}


void Print::save_to(std::string path)
{
	g_autofree guchar *buffer = nullptr;
	gsize length = 0;
	g_autoptr(GError) error = nullptr;

	gboolean worked = fp_print_serialize(print_ptr, &buffer, &length, &error);

	if (!worked) {
		debug("Não consegui salvar: ", error->message);
		return;
	}

	std::ofstream output(path, std::ios::out | std::ios::binary);
	output.write(reinterpret_cast<const char*>(&length), sizeof(gsize));
	output.write(reinterpret_cast<const char*>(buffer), sizeof(guchar)*length);
	debug("File saved to ", path);
}
