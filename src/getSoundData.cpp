#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "getSoundData.h"


/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int recordCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	paTestData *data = (paTestData*)userData;
	const SAMPLE *rptr = (const SAMPLE*)inputBuffer;


	(void)outputBuffer; /* Prevent unused variable warnings. */
	(void)timeInfo;
	(void)statusFlags;
	(void)userData;

	memcpy(data->getWritePointer(), inputBuffer, framesPerBuffer*sizeof(SAMPLE));
	data->flip();

	return paContinue;
}

void mySound::startStream()
{
	err = Pa_StartStream(stream);
	if (err != paNoError)
	{
		printf("Error starting stream\n");
		return;
	}
}

void mySound::endStream()
{
	err = Pa_CloseStream(stream);
	if (err != paNoError)
	{
		printf("Error closing stream\n");
	}
	printf("Successfully closed Stream!\n");
}




mySound::~mySound()
{


	err = Pa_StopStream(stream);
	if (err != paNoError)
	{
		printf("GetSoundData.cpp: Error stopping stream!\n");
	}


	err = Pa_CloseStream(stream);
	if (err != paNoError)
	{
		printf("GetSoundData.cpp: Error closing stream!\n");
	}

	Pa_Terminate();
	if (data.firstBuffer)       /* Sure it is NULL or valid. */
		free(data.firstBuffer);
	if (data.secondBuffer)       /* Sure it is NULL or valid. */
		free(data.secondBuffer);

	if (err != paNoError)
	{
		fprintf(stderr, "An error occured while using the portaudio stream\n");
		fprintf(stderr, "Error number: %d\n", err);
		fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
	}
	return;

}


mySound::mySound()
{
}


void mySound::destroy()
{
	err = Pa_StopStream(stream);
	if (err != paNoError)
	{
		printf("GetSoundData.cpp: Error stopping stream!\n");
	}


	err = Pa_CloseStream(stream);
	if (err != paNoError)
	{
		printf("GetSoundData.cpp: Error closing stream!\n");
	}

	Pa_Terminate();
	if (data.firstBuffer)       /* Sure it is NULL or valid. */
		free(data.firstBuffer);
	if (data.secondBuffer)       /* Sure it is NULL or valid. */
		free(data.secondBuffer);

	if (err != paNoError)
	{
		fprintf(stderr, "An error occured while using the portaudio stream\n");
		fprintf(stderr, "Error number: %d\n", err);
		fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
	}
	return;

}



void mySound::init()
{


	int numSamples = FRAMES_PER_BUFFER * NUM_CHANNELS;
	int numBytes = numSamples * sizeof(SAMPLE);

	data.first = true;
	data.notified = false;

	data.firstBuffer = (SAMPLE *)calloc(1, numBytes); /* From now on, recordedSamples is initialised. */
	data.secondBuffer = (SAMPLE *)calloc(1, numBytes);
	if (data.firstBuffer == NULL || data.secondBuffer == NULL)
	{
		printf("GetSoundData.cpp: Could not allocate record array of size %d bytes\n", numBytes);
		return;
	}


	err = Pa_Initialize();
	if (err != paNoError)
	{
		printf("GetSoundData.cpp: Failed to initialize port audio!\n");
		return;
	}

	inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
	//inputParameters.device = 19; /* default input device */
	if (inputParameters.device == paNoDevice) {
		fprintf(stderr, "GetSoundData.cpp: Error: No default input device.\n");
		return;
	}

    // Device Info for debugging
    const PaDeviceInfo *deviceInfo;
    deviceInfo = Pa_GetDeviceInfo(inputParameters.device);
    printf("Name        =   %s\n", deviceInfo->name);
    printf("Host API    =   %s\n", Pa_GetHostApiInfo(deviceInfo->hostApi)->name);
    printf( "Default low input latency   = %8.4f\n", deviceInfo->defaultLowInputLatency  );
    printf( "Default low output latency  = %8.4f\n", deviceInfo->defaultLowOutputLatency  );
    printf( "Default high input latency  = %8.4f\n", deviceInfo->defaultHighInputLatency  );
    printf( "Default high output latency = %8.4f\n", deviceInfo->defaultHighOutputLatency  );
    printf( "Default sample rate         = %8.2f\n", deviceInfo->defaultSampleRate );


	inputParameters.channelCount = NUM_CHANNELS;                    /* stereo input */
	inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;



	/* Record some audio. -------------------------------------------- */
    printf("Test recording\n");
	err = Pa_OpenStream(
		&stream,
		&inputParameters,
		NULL,                  /* &outputParameters, */
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		recordCallback,
		&data);
	if (err != paNoError)
	{
		printf("Error opening stream\n");
		return;
	}

	return;
}
