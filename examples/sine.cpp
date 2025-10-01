#include <cmath>
#include <iostream>
#include <portaudio.hpp>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

constexpr int SAMPLE_RATE = 44100;
constexpr int FRAMES_PER_BUFFER = 256;

struct paData
{
	double sine_freq;
	double phase_step;
	double phase = 0.0;
};

static int patestCallback(
	const void *inputBuffer,
	void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo *timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	(void)inputBuffer; // Prevent unused variable warning.
	auto *data = static_cast<paData *>(userData);
	auto *out = static_cast<float *>(outputBuffer);

	for (unsigned long i = 0; i < framesPerBuffer; i++)
	{
		*out++ = static_cast<float>(sin(data->phase)); // Left channel
		*out++ = static_cast<float>(sin(data->phase)); // Right channel
		data->phase += data->phase_step;
		if (data->phase >= 2.0 * M_PI)
			data->phase -= 2.0 * M_PI;
	}
	return paContinue;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <frequency_in_hz>" << std::endl;
		return 1;
	}

	try
	{
		paData data;
		data.sine_freq = std::stod(argv[1]);
		data.phase_step = 2.0 * M_PI * data.sine_freq / SAMPLE_RATE;

		pa::Init init;

		pa::Stream stream(0, 2, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, patestCallback, &data);

		stream.start();

		std::cout << "Playing a sine wave of " << data.sine_freq << " Hz. Press Enter to stop." << std::endl;
		std::cin.get();

		stream.stop();
	}
	catch (const pa::Error &e)
	{
		std::cerr << "PortAudio error: " << e.what() << std::endl;
		return 1;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
