#pragma once
#include <RtAudio.h>
#include "buffer.h"
#include "FIFO_Array.h"


ThreadSafeQ<buffer> fifo(441);

int readSpeakerLoopback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
    double streamTime, RtAudioStreamStatus status, void* data)
{
    int numOfChannels = 2;
    std::vector<float> copy;
    for (int frame = 0; frame < nBufferFrames * numOfChannels; frame++) {
        float sample = ((float*)inputBuffer)[frame];
        copy.push_back(sample);
    }
    buffer input = buffer(copy);
    fifo.push(input);
    return false;
}

int playSpeakerSound(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
    double streamTime, RtAudioStreamStatus status, void* data)
{
    int numOfChannels = 2;
    buffer output = buffer();
    fifo.pop(output);
    std::vector<float> temp = output.dataBuffer;
    float* playback = (float*)outputBuffer;
    for (int frame = 0; frame < nBufferFrames * numOfChannels; frame++) {
        *playback++ = temp.at(frame);
    }
    return 0;
}
