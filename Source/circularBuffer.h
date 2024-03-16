/*
  ==============================================================================

    circularBuffer.h
    Created: 30 Apr 2021 12:34:39pm
    Author:  josea

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class combFilter
{
public:

    combFilter():
        Fs(44100), maxDelayTime(2), delayTime(1), numberOfChannels(2)
    {
        bufferSize = (maxDelayTime) * Fs;
        bufferForY = new float* [numberOfChannels];
        for (int channel = 0; channel < numberOfChannels; ++channel)
        {
            bufferForY[channel] = new float[bufferSize];
        }

        n_D = new int[numberOfChannels];
        n = new int[numberOfChannels];

        updateParameters();
    }

    /*combFilter(int numberOfInputChannels, int sR, float Tmax)
    {
        Fs = sR;
        maxDelayTime = Tmax;
        numberOfChannels = numberOfInputChannels;

        if (maxDelayTime > 1) { delayTime = 1; }
        else { delayTime = maxDelayTime / 2; }
        
        bufferSize = (maxDelayTime) * Fs;
        buffer = new float* [numberOfChannels];
        for (int channel = 0; channel < numberOfChannels; ++channel)
        {
            buffer[channel] = new float[bufferSize];
        }

        n_D = new int[numberOfChannels];
        n = new int[numberOfChannels];

        updateParameters();

    }*/

    void updateParameters()
    {
         for (int channel = 0; channel < numberOfChannels; ++channel)
        {
            if (bufferForY[channel] != nullptr)
                delete[] bufferForY[channel];
        }
        if (bufferForY != nullptr)
            delete[] bufferForY;
        if (n_D != nullptr)
            delete[] n_D;
        if (n != nullptr)
            delete[] n;


        bufferSize = ( maxDelayTime) * Fs;


        bufferForY = new float* [numberOfChannels];
        n_D = new int[numberOfChannels];
        n = new int[numberOfChannels];
        for (int channel = 0; channel < numberOfChannels; ++channel)
        {
            bufferForY[channel] = new float[bufferSize];
            for (int n = 0; n < bufferSize; ++n)
            {
                bufferForY[channel][n] = 0;
            }
            n[channel] = 0;
            n_D[channel] = n[channel] - Fs * delayTime + bufferSize;
        }
    }

    void updateParameters(int numberOfInputChannels, int sR, float Tmax)
    {
        for (int channel = 0; channel < numberOfChannels; ++channel)
        {
            if(bufferForY[channel]!= nullptr)
                delete[] bufferForY[channel];
        }
        if(bufferForY!= nullptr)
            delete[] bufferForY;
        if (n_D != nullptr)
            delete[] n_D;
        if (n != nullptr)
            delete[] n;

        Fs = sR;
        maxDelayTime = Tmax;
        if (delayTime >= maxDelayTime)
            delayTime = maxDelayTime;

        numberOfChannels = numberOfInputChannels;

        bufferSize = (maxDelayTime) * Fs;

        //buffer.resize(numberOfChannels, std::vector<float>(bufferSize, 0));

        bufferForY = new float* [numberOfChannels];
        n_D = new int[numberOfChannels];
        n = new int[numberOfChannels];
        for (int channel = 0; channel < numberOfChannels; ++channel)
        {
            bufferForY[channel] = new float[bufferSize];
            for (int n = 0; n < bufferSize; ++n)
            {
                bufferForY[channel][n] = 0;
            }
            n[channel] = 0;
            n_D[channel] = n[channel] - Fs * delayTime + bufferSize;
        }
    }

    float requestSample(int channel,float xn,float feedBack)
    {
        float oldestSample = bufferForY[channel][n_D[channel]];

        bufferForY[channel][n[channel]] = xn + feedBack*oldestSample;

        ++n[channel];
        if (n[channel] >= (bufferSize - 1)){ n[channel] = 0; }
            
        n_D[channel] = n[channel] - Fs * delayTime;
        if (n_D[channel] < 0) { n_D[channel] = n_D[channel] + bufferSize; }

        return oldestSample;
    }

    void updateDelayTime(float newDelayTime)
    {
        if (newDelayTime >= maxDelayTime)
            delayTime = maxDelayTime;
        else
            delayTime = newDelayTime;
    }

    void updateMaxDelayTime(float newTmax)
    {
        maxDelayTime = newTmax;
    }

    void updateSampleRate(int newSampleRate)
    {
        Fs = newSampleRate;
        updateParameters();
    }

    void updatenumberOfChannels(int numberOfInputChannels)
    {
        numberOfChannels = numberOfInputChannels;
        updateParameters();
    }

    ~combFilter() 
    {
        for (int channel = 0; channel < numberOfChannels; ++channel)
        {
            delete[] bufferForY[channel];
        }
        delete[] bufferForY;
        delete[] n_D;
        delete[] n;
    }

//protected:

    int Fs, bufferSize, numberOfChannels;
    float maxDelayTime;

    std::atomic<float> delayTime;
    int *n_D, *n;
    //std::vector<std::vector<float>> buffer;

    float **bufferForY;
};

//class allpassFilter : public combFilter
//{
//public:
//    float requestSample(int channel, float xn, float g)
//    {
//        float yn = buffer[channel][n_D[channel]];
//
//        buffer[channel][n[channel]] = xn + g * yn;
//
//        ++n[channel];
//        if (n[channel] >= (bufferSize - 1)) { n[channel] = 0; }
//
//        n_D[channel] = n[channel] - Fs * delayTime;
//        if (n_D[channel] < 0) { n_D[channel] = n_D[channel] + bufferSize; }
//
//        return yn;
//    }
//};

        /*bufferForX.resize(numberOfChannels);
        bufferForY.resize(numberOfChannels);
        for (int channel = 0;channel<numberOfChannels;++channel)
        {
            bufferForX[channel].reset(new float[bufferSize]);
            bufferForY[channel].reset(new float[bufferSize]);
            for (int n = 0; n < bufferSize; ++n)
            {
                bufferForX[channel].get()[n] = 0;
            }
        }*/