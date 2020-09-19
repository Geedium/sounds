#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include <Windows.h>

#include <iostream>
using namespace std;

#include <fstream>
#include <string>


#include "Wave.hpp"

ALbyte buffer[22050];
ALint sample;

int main()
{
    alGetError();
    ALCdevice* device = alcCaptureOpenDevice(NULL, 44100, AL_FORMAT_STEREO16, 1024);
    if (alGetError() != AL_NO_ERROR) {
        return 0;
    }
    alcCaptureStart(device);

    Wave* wave = new Wave("mic.wav");

    bool isTerminated = false;

    while (!isTerminated)
    {
        alcGetIntegerv(device, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(ALint), &sample);
        alcCaptureSamples(device, (ALCvoid*)buffer, sample);

        if (alcGetError(device) != ALC_NO_ERROR)
        {
            exit(0);
        }       

        if (sample != 0)
        {
            wave->Add(&sample);
            for (int i = 0; i < 22050; i++)
            {
                wave->Write((ALuint)buffer[i]);
            }
        }

        if (GetAsyncKeyState(VK_DELETE))
        {
            isTerminated = true;
            wave->Out();
        }
    }

    std::cout << "Operations completed." << std::endl;

    while (true)
    {

    }

    delete wave;

    alcCaptureStop(device);
    alcCaptureCloseDevice(device);
	return 0;
}