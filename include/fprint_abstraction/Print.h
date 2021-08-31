/*
 * Print.h
 *
 *  Created on: 22 de abr. de 2021
 *      Author: thiago
 */

#ifndef INCLUDE_FPRINT_ABSTRACTION_PRINT_H_
#define INCLUDE_FPRINT_ABSTRACTION_PRINT_H_

extern "C"
{
#include <libfprint-2/fprint.h>
};

#include <string>
#include <sstream>

#include <fprint_abstraction/utils.h>
#include <fprint_abstraction/Device.h>


class Print
{
	public:
		Print() = delete;
		Print(Print& other);
		Print(Print&& other);

//		Print& operator=(Print& other);
		Print& operator=(Print&& other);


		virtual ~Print();

		std::string get_driver();
		std::string get_device_id();

		void set_finger(FpFinger finger);
		FpFinger get_finger();

		std::string get_username();
		void set_username(std::string& username);

		std::string get_description();
		void set_description(std::string& description);

		void set_enroll_date_now();
		void set_enroll_date(int day, int month, int year);
		utils::simple_date get_enroll_date();

		bool equal(Print& other);
		utils::serialized_print serialize();

		static Print deserialize(utils::serialized_print& stream);

		// Temporary Function - should be deleted soon
		void save_to(std::string path);

	private:
		friend class Device;
		Print(FpDevice *device);
		Print(FpPrint *print);

	private:
		FpPrint *print_ptr;
};

#endif /* INCLUDE_FPRINT_ABSTRACTION_PRINT_H_ */
