/*
 * FContext.h
 *
 *  Created on: 22 de abr. de 2021
 *      Author: thiago
 */


#ifndef INCLUDE_FPRINT_ABSTRACTION_CONTEXT_H_
#define INCLUDE_FPRINT_ABSTRACTION_CONTEXT_H_


extern "C"
{
#include <libfprint-2/fprint.h>
};

#include <fprint_abstraction/Device.h>


class Context
{
	public:
		Context() = delete;
		Context(Context&& ctx);
		Context& operator=(Context&& ctx);
		virtual ~Context();

		guint n_devices();
		Device get_device(guint index);

		static Context context_new();

	private:
		Context(FpContext *pointer);
		FpContext *ctx_ptr;  // transfer full
};


#endif /* INCLUDE_FPRINT_ABSTRACTION_CONTEXT_H_ */
