#pragma once

#include <portaudio.h>
#include <stdexcept>

namespace pa
{

/**
 * Exception struct wrapping a `PaError` and extending `std::runtime_error`.
 * Constructs the `what()` message using `Pa_GetErrorText`.
 */
struct Error : std::runtime_error
{
	const PaError code;
	Error(const std::string &func, const PaError code)
		: std::runtime_error{func + ": " + Pa_GetErrorText(code)},
		  code{code}
	{
	}
};

} // namespace pa
