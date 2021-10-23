/*
 * FDevice.cpp
 *
 *  Created on: 22 de abr. de 2021
 *      Author: thiago
 */

#include <fprint_abstraction/Device.h>
#include <fprint_abstraction/utils.h>
#include <fprint_abstraction/utils.h>
#include <fprint_abstraction/exceptions/Error.h>

#include <string>


struct EnrollProgressCallbackData
{
	std::function<Device::EnrollProgressCallbackType>& callback;
};


static FpPrint* from_print_template(FpDevice *device, PrintTemplate& print_template)
{
	FpPrint *print = fp_print_new(device);

	if (print_template.get_finger() != FP_FINGER_UNKNOWN) {
		fp_print_set_finger(print, print_template.get_finger());
	}

	if (!print_template.get_username().empty()) {
		g_autofree gchar *c_username = utils::string_to_char(print_template.get_username());
		fp_print_set_username(print, c_username);
	}

	if (!print_template.get_description().empty()) {
		g_autofree char *c_description = utils::string_to_char(print_template.get_description());
		fp_print_set_description(print, c_description);
	}

	if (print_template.get_enroll_date().day != 0
			|| print_template.get_enroll_date().month != 0
			|| print_template.get_enroll_date().year != 0) {
		g_autoptr(GDate) date = utils::new_gdate(print_template.get_enroll_date().day,
												 print_template.get_enroll_date().month,
												 print_template.get_enroll_date().year);
		fp_print_set_enroll_date(print, date);
	}

	return print;
}



Device::Device(FpDevice *dev)
: dev_ptr(dev)
{}


Device::Device(Device&& dev)
: dev_ptr(dev.dev_ptr)
{
	debug("Moving Device");
	dev.dev_ptr = nullptr;
}


Device& Device::operator=(Device&& dev)
{
	debug("Moving Device by assigment");
	this->dev_ptr = dev.dev_ptr;
	dev.dev_ptr = nullptr;
	return *this;
}


Device::~Device()
{
	if (dev_ptr) debug("Cleanup device");

	RELEASE_FPDEVICE(dev_ptr);
	dev_ptr = nullptr;
}


std::string Device::get_driver()
{
	return fp_device_get_driver(dev_ptr);
}


std::string Device::get_device_id()
{
	return fp_device_get_device_id(dev_ptr);
}


std::string Device::get_name()
{
	return fp_device_get_name(dev_ptr);
}


int	Device::get_nr_enroll_stages()
{
	return fp_device_get_nr_enroll_stages(dev_ptr);
}


Print Device::enroll()
{
	auto lambda = [](FpDevice *device,
                     gint completed_stages,
                     FpPrint *print,
                     gpointer data,
                     GError *error)
	{
		if (error) {
			debug("Fail with message: ", error->message);
			return;
		}
		debug("Stage ", completed_stages, " from ", fp_device_get_nr_enroll_stages(device),
			  ". Internal print is ", print, " and data is ", data);
	};

	g_autoptr(GError) error = nullptr;
	g_autoptr(FpPrint) tmplt = fp_print_new(dev_ptr);

	FpPrint *fp_print = fp_device_enroll_sync(dev_ptr,
			                                  tmplt,
											  nullptr,
											  (FpEnrollProgress) lambda,
											  nullptr,
											  &error);

	debug("Created FpPrint [", fp_print, "]");

	if (error) {
		debug(error->message);
		throw Error(error->message);
	}

	if (tmplt == fp_print) {
		tmplt = nullptr;
	}

	return fp_print;
}


Print Device::enroll(PrintTemplate &print_template)
{
	auto lambda = [](FpDevice *device,
                     gint completed_stages,
                     FpPrint *print,
                     gpointer data,
                     GError *error)
	{
		if (error) {
			debug("Fail with message: ", error->message);
			return;
		}
		debug("Stage ", completed_stages, " from ", fp_device_get_nr_enroll_stages(device),
			  ". Internal print is ", print, " and data is ", data);
	};

	g_autoptr(GError) error = nullptr;
	g_autoptr(FpPrint) tmplt = from_print_template(dev_ptr, print_template);

	FpPrint *fp_print =	fp_device_enroll_sync(dev_ptr,
			                                  tmplt,
											  nullptr,
											  (FpEnrollProgress) lambda,
											  nullptr,
											  &error);

	debug("Created FpPrint [", fp_print, "]");

	if (error) {
		debug(error->message);
		throw Error("Deu foi pau!");
	}

	if (tmplt == fp_print) {
		tmplt = nullptr;
	}

	return fp_print;
}


Print Device::enroll(std::function<EnrollProgressCallbackType> callback, PrintTemplate &print_template)
{
	auto lambda = [](FpDevice *device,
                     gint completed_stages,
                     FpPrint *print,
                     gpointer user_data,
                     GError *error)
	{
		if (error) {
			debug("Fail with message: ", error->message);
			return;
		}
		debug("Stage ", completed_stages, " from ", fp_device_get_nr_enroll_stages(device),
			  ". Internal print is ", print, " and data is ", user_data);

		EnrollProgressCallbackData *data = reinterpret_cast<EnrollProgressCallbackData*>(user_data);
		data->callback(device, completed_stages, print, data, error);
	};

	g_autoptr(GError) error = nullptr;
	g_autoptr(FpPrint) tmplt = from_print_template(dev_ptr, print_template);
	EnrollProgressCallbackData data{callback};

	FpPrint *fp_print = fp_device_enroll_sync(dev_ptr,
	                                          tmplt,
											  nullptr,
											  (FpEnrollProgress) lambda,
											  &data,
											  &error);
	
	debug("Created FpPrint [", fp_print, "]");

	if (error) {
		debug(error->message);
		throw Error("Deu foi pau!");
	}

	if (tmplt == fp_print) {
		tmplt = nullptr;
	}

	return fp_print;
}


bool Device::verify(Print& print)
{
	auto lambda = [](FpDevice *device,
			         FpPrint *match,
					 FpPrint *print,
					 gpointer data,
					 GError *error)
    {
		debug("Print: ", fp_print_get_device_id(print));
		debug("Device: ", fp_device_get_name(device));
		debug("Data null? ", data ? "no": "yes");

		if (error) {
			debug("Fail with message: ", error->message);
			return;
		}

		if (match) {
			debug("Match!");
		} else {
			debug("Does not match");
		}
    };

	gboolean match = true;
	g_autoptr(GError) error = nullptr;

	gboolean ok = fp_device_verify_sync(dev_ptr,
										print.print_ptr,
										nullptr,
										lambda,
										nullptr,
										&match,
										nullptr,
										&error);

	if (error) {
		debug("Fail with: ", error->message);
		return false;
	} else if (!ok) { // throw exception?
		debug("Fail");
		return false;
	}

	debug("ok is ", ok ? "true" : "false");

	return static_cast<bool>(match);
}


bool Device::compatible(Print& print)
{
	return static_cast<bool>(fp_print_compatible(print.print_ptr, dev_ptr));
}


bool Device::open()
{
	g_autoptr(GError) error = nullptr;
	gboolean worked = fp_device_open_sync(dev_ptr, nullptr, &error);
	if (error) {
		debug(error->message);
	}
	return worked;
}


bool Device::close()
{
	g_autoptr(GError) error = nullptr;
	gboolean worked = fp_device_close_sync(dev_ptr, nullptr, &error);
	if (error) {
		debug(error->message);
	}
	return worked;
}
