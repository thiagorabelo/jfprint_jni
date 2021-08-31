
/*
 * FContext.cpp
 *
 *  Created on: 22 de abr. de 2021
 *      Author: thiago
 */

extern "C"
{
#include <glib-unix.h>
}

#include <fprint_abstraction/Context.h>
#include <fprint_abstraction/utils.h>
#include <fprint_abstraction/exceptions/Error.h>


Context::Context(FpContext *pointer)
: ctx_ptr(pointer)
{}


Context::Context(Context&& ctx)
: ctx_ptr(ctx.ctx_ptr)
{
	debug("Moving Context");
	ctx.ctx_ptr = nullptr;
}


Context& Context::operator=(Context&& ctx)
{
	debug("Moving Context by assigment");
	this->ctx_ptr = ctx.ctx_ptr;
	ctx.ctx_ptr = nullptr;
	return *this;
}


Context::~Context()
{
	if (ctx_ptr) debug("Cleanup context");

	RELEASE_FPCONTEXT(ctx_ptr);
	ctx_ptr = nullptr;
}


guint Context::n_devices()
{
	GPtrArray *array = fp_context_get_devices(ctx_ptr);
	return array ? array->len : 0;
}


Device Context::get_device(guint index)
{
	if (!n_devices()) {
		throw Error("No device found");
	}

	GPtrArray *array = fp_context_get_devices(ctx_ptr);
	FpDevice *dev = static_cast<FpDevice*>(g_ptr_array_index(array, index));
	return Device(dev);
}


Context Context::context_new()
{
	debug("Creating context");
	FpContext *ctx = fp_context_new();
	return Context(ctx);
}
