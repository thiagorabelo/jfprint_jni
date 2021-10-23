/*
 * FDevice.h
 *
 *  Created on: 22 de abr. de 2021
 *      Author: thiago
 */

#ifndef INCLUDE_FPRINT_ABSTRACTION_DEVICE_H_
#define INCLUDE_FPRINT_ABSTRACTION_DEVICE_H_


class Print;


extern "C"
{
#include <libfprint-2/fprint.h>
};

#include <string>
#include <functional>

#include <fprint_abstraction/PrintTemplate.h>
#include <fprint_abstraction/Print.h>


class Device
{
	public:
		Device() = delete;
        Device(FpDevice *device);
		Device(Device&& dev);
		Device& operator=(Device&& dev);
		virtual ~Device();

		std::string get_driver();
		std::string get_device_id();
		std::string get_name();

		int	get_nr_enroll_stages();
		Print enroll();
		Print enroll(PrintTemplate &print_template);

		using EnrollProgressCallbackType = void(FpDevice*, gint, FpPrint*, gpointer, GError*);

		Print enroll(std::function<EnrollProgressCallbackType> callback, PrintTemplate &print_template);

		bool verify(Print& print);

		bool compatible(Print &print);

		bool open();
		bool close();

	private:
		friend class Context;

	private:
		FpDevice *dev_ptr;
};

#endif /* INCLUDE_FPRINT_ABSTRACTION_DEVICE_H_ */
