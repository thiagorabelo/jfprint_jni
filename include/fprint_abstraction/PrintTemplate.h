/*
 * PrintTemplate.h
 *
 *  Created on: 19 de mai. de 2021
 *      Author: thiago
 */

#ifndef INCLUDE_FPRINT_ABSTRACTION_PRINTTEMPLATE_H_
#define INCLUDE_FPRINT_ABSTRACTION_PRINTTEMPLATE_H_

extern "C"
{
#include <libfprint-2/fprint.h>
};

#include <string>

#include <fprint_abstraction/utils.h>


class PrintTemplate
{
	public:
		PrintTemplate();
		PrintTemplate(PrintTemplate& tmplt);
		PrintTemplate(PrintTemplate&& tmplt);

		virtual ~PrintTemplate();

		void set_finger(FpFinger finger);
		FpFinger get_finger();

		std::string& get_username();
		void set_username(std::string username);

		std::string& get_description();
		void set_description(std::string description);

		void set_enroll_date_now();
		void set_enroll_date(int day, int month, int year);
		utils::simple_date& get_enroll_date();

	private:
		friend class Device;

		FpFinger finger;
		std::string username;
		std::string description;
		utils::simple_date enroll_date;
};

#endif /* INCLUDE_FPRINT_ABSTRACTION_PRINTTEMPLATE_H_ */
