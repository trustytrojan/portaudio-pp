#pragma once

#include <iostream>
#include "Error.hpp"

#include <portaudio.h>

namespace pa
{
	class Stream
	{
		PaStream *_s;

	public:
		/**
		 * Wrapper over `Pa_OpenDefaultStream`.
		 * @throws `pa::Error` if `Pa_OpenDefaultStream` fails
		 */
		Stream(
			int numInputChannels,
			int numOutputChannels,
			PaSampleFormat sampleFormat,
			double sampleRate,
			unsigned long framesPerBuffer,
			PaStreamCallback *streamCallback = NULL,
			void *userData = NULL)
		{
			if (const auto rc = Pa_OpenDefaultStream(&_s, numInputChannels, numOutputChannels, sampleFormat, sampleRate, framesPerBuffer, streamCallback, userData))
				throw Error("Pa_OpenDefaultStream", rc);
		}

		/**
		 * Wrapper over `Pa_OpenStream`.
		 * @throws `pa::Error` if `Pa_OpenStream` fails
		 */
		Stream(
			const PaStreamParameters *inputParameters,
			const PaStreamParameters *outputParameters,
			double sampleRate,
			unsigned long framesPerBuffer,
			PaStreamFlags streamFlags = 0,
			PaStreamCallback *streamCallback = NULL,
			void *userData = NULL)
		{
			if (const auto rc = Pa_OpenStream(&_s, inputParameters, outputParameters, sampleRate, framesPerBuffer, streamFlags, streamCallback, userData))
				throw Error("Pa_OpenStream", rc);
		}

		~Stream()
		{
			if (!_s)
				return;
			if (const auto rc = Pa_CloseStream(_s))
				std::cerr << "Pa_CloseStream: " << Pa_GetErrorText(rc) << '\n';
		}

		// Delete copy constructor
		Stream(const Stream &) = delete;

		// Delete copy assignment operator
		Stream &operator=(const Stream &) = delete;

		// Move constructor
		Stream(Stream &&other)
		{
			// close this stream
			this->~Stream();

			// take `other`'s stream, making sure its destructor call won't error
			_s = other._s;
			other._s = nullptr;
		}

		// Move assignment operator
		Stream &operator=(Stream &&other)
		{
			// close this stream
			this->~Stream();

			// take `other`'s stream, making sure its destructor call won't error
			_s = other._s;
			other._s = nullptr;

			return *this;
		}

		/**
		 * Wrapper over `Pa_StartStream`.
		 * @throws `pa::Error` if `Pa_StartStream` fails
		 */
		void start()
		{
			if (const auto rc = Pa_StartStream(_s))
				throw Error("Pa_StartStream", rc);
		}

		/**
		 * Wrapper over `Pa_StopStream`.
		 * @throws `pa::Error` if `Pa_StopStream` fails
		 */
		void stop()
		{
			if (const auto rc = Pa_StopStream(_s))
				throw Error("Pa_StopStream", rc);
		}

		/**
		 * Wrapper over `Pa_AbortStream`.
		 * @throws `pa::Error` if `Pa_AbortStream` fails
		 */
		void abort()
		{
			if (const auto rc = Pa_AbortStream(_s))
				throw Error("Pa_AbortStream", rc);
		}

		/**
		 * Write samples to an output stream. This function doesn't return
		 * until the entire buffer has been written - this may involve
		 * waiting for the operating system to consume the data.
		 *
		 * @param buffer A pointer to a buffer of sample frames.
		 * If non-interleaved samples were requested using the `paNonInterleaved`
		 * sample format flag, buffer is a pointer to the first element of an
		 * array of buffer pointers, one non-interleaved buffer for each channel.
		 *
		 * @param frames The number of frames to be written from buffer.
		 * This parameter is not constrained to a specific range, however
		 * high performance applications will want to match this parameter
		 * to the framesPerBuffer parameter used when opening the stream.
		 * 
		 * @throws `pa::Error` if `Pa_WriteStream` returns a non-zero value
		 */
		void write(const void *const buffer, const size_t frames)
		{
			if (const auto rc = Pa_WriteStream(_s, buffer, frames))
				throw Error("Pa_WriteStream", rc);
		}
	};
}