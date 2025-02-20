#include "portaudio.h"
#include <thread>
#include <mutex>
#include <condition_variable>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#else
#include "pa_linux_alsa.h"
#endif

/* #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (1024)
#define NUM_CHANNELS    (1)
/* #define DITHER_FLAG     (paDitherOff) */
#define DITHER_FLAG     (0) /**/
/** Set to 1 if you want to capture the recording to a file. */
#define WRITE_TO_FILE   (0)


/* Select sample format. */
#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE  (128)
#define PRINTF_S_FORMAT "%d"
#endif

typedef struct
{
	SAMPLE      *firstBuffer;
	SAMPLE      *secondBuffer;
	std::condition_variable flipped;
	std::mutex m;
	bool		first;
	bool notified;
	SAMPLE*		getWritePointer()
	{
		return first ? firstBuffer : secondBuffer;
	}

	SAMPLE*		getReadPointer()
	{
		return first ? secondBuffer : firstBuffer;
	}


	void flip()
	{
		first = !first;
		notified = true;
		flipped.notify_one();
	}

}paTestData;

class mySound
{

public:
	mySound();
	~mySound();

	void startStream();
	void endStream();

	paTestData          data;

	void init();
	void destroy();

private:
	PaStreamParameters  inputParameters;
	PaStream*           stream;
	PaError             err = paNoError;
};
