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
struct PortAudio
{
	PortAudio()
	{
		if (const auto rc = Pa_Initialize())
			throw Error("Pa_Initialize", rc);
	}

	~PortAudio()
	{
		if (const auto rc = Pa_Terminate())
			std::cerr << "Pa_Terminate: " << Pa_GetErrorText(rc) << '\n';
	}

	// The library should only be initialized once.
	// It does not make sense to copy or move a `PortAudio` instance.

	PortAudio(const PortAudio &) = delete;
	PortAudio &operator=(const PortAudio &) = delete;
	PortAudio(PortAudio &&) = delete;
	PortAudio &operator=(PortAudio &&) = delete;
};

}; // namespace pa
