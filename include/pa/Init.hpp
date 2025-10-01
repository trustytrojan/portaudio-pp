#pragma once

#include "Error.hpp"
#include <iostream>
#include <portaudio.h>

namespace pa
{

/**
 * RAII wrapper over `Pa_Initialize` and `Pa_Terminate`.
 * You must own an instance of `PortAudio` before doing anything else with the library.
 */
struct Init
{
	Init()
	{
		if (const auto rc = Pa_Initialize())
			throw Error{"Pa_Initialize", rc};
	}

	~Init()
	{
		if (const auto rc = Pa_Terminate())
			std::cerr << "Pa_Terminate: " << Pa_GetErrorText(rc) << '\n';
	}

	// The library should only be initialized once.
	// It does not make sense to copy or move a `PortAudio` instance.

	Init(const Init &) = delete;
	Init &operator=(const Init &) = delete;
	Init(Init &&) = delete;
	Init &operator=(Init &&) = delete;
};

}; // namespace pa
