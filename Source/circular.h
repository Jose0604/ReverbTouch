/*
  ==============================================================================

    circular.h
    Created: 5 May 2021 8:56:12pm
    Author:  josea

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class circularBuffer
{
public:
    circularBuffer():
        bufferSize(1024),D(512)
    {
        n = nullptr;
        bufferForX = new float* [bufferSize]();
        bufferForY = new float* [bufferSize]();
        updateParameters(5, 2, 44100);
    }

    ~circularBuffer()
    {
        for (int channel = 0; channel < numberOfChannels; ++channel)
        {
            delete[] bufferForX[channel];
            delete[] bufferForY[channel];
        }
        delete[] bufferForY;
        delete[] bufferForX;
        delete[] n;
    }

    void updateParameters(float Tmax, int numberOfInputChannels = 2, int sR = 44100)
    {

        for (int channel = 0; channel < numberOfChannels; ++channel)
        {
            if (bufferForY[channel] != nullptr) { delete[] bufferForY[channel]; }
            if (bufferForX[channel] != nullptr) { delete[] bufferForX[channel]; }
                
        }
        if (bufferForY != nullptr) { delete[] bufferForY; }
        if (bufferForX != nullptr) { delete[] bufferForX; }
        if (n != nullptr) { delete[] n; }
            
        Fs = sR;

        bufferSize = Tmax * Fs;
        if (D >= bufferSize) { D = bufferSize; }

        numberOfChannels = numberOfInputChannels;

        n = new int[numberOfChannels];
        
        bufferForX = new float* [numberOfChannels];
        bufferForY = new float* [numberOfChannels];
        for (int channel = 0; channel < numberOfChannels; channel++)
        {
            bufferForX[channel] = new float[bufferSize];
            bufferForY[channel] = new float[bufferSize];

            for (int n = 0; n < bufferSize; n++)
            {
                bufferForX[channel][n] = 0;
                bufferForY[channel][n] = 0;
            }
            n[channel] = 0;
        }
       
    }
    
    float requestSample(int channel, float x, float g)
    {
        jassert(g < 1);

        float y;

        int n_D = n[channel] - D;
        if (n_D < 0) { n_D += bufferSize; }

        y = bufferForX[channel][n_D] + g * bufferForY[channel][n_D];
        bufferForX[channel][n[channel]] = x;
        bufferForY[channel][n[channel]] = y;

        ++n[channel];
        if (n[channel] >= (bufferSize - 1)) { n[channel] = 0; }

        return y;
    }

    float requestSample(bool isAPF, int channel, float x, float g)
    {
        jassert(g < 1);

        float y;

        int n_D = n[channel] - D;
        if (n_D < 0) { n_D += bufferSize; }

        y = -g*x+bufferForX[channel][n_D]+g* bufferForY[channel][n_D];
        bufferForX[channel][n[channel]] = x;
        bufferForY[channel][n[channel]] = y;

        ++n[channel];
        if (n[channel] >= (bufferSize - 1)) { n[channel] = 0; }

        return y;
    }



    void updateDelayTime(double newDelayTime)
    {
        if (newDelayTime*Fs >= bufferSize) { D = bufferSize; }
        else { D =  newDelayTime * Fs; }            
    }

private:

    int Fs, bufferSize, numberOfChannels;
    std::atomic<int> D;

    int* n;
    float** bufferForY, ** bufferForX;
};