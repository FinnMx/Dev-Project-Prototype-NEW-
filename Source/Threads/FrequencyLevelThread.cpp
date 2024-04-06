/*
  ==============================================================================

    FrequencyLevelThread.cpp
    Created: 6 Apr 2024 9:56:06pm
    Author:  New Owner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FrequencyLevelThread.h"

//==============================================================================
FrequencyLevelThread::FrequencyLevelThread(float* frequencies) : juce::Thread("Frequency Level Thread"), frequencies(frequencies)
{ 
    buffer.clear();

}

FrequencyLevelThread::~FrequencyLevelThread()
{
}

void FrequencyLevelThread::timerCallback() {
    elapsedTime++;
}

void FrequencyLevelThread::run() {
    startTimer(1000);

    while(elapsedTime <= 5){
        auto tempBuffer = buffer; // Input audio buffer
        const int numSamples = tempBuffer.getNumSamples();
        const int numChannels = tempBuffer.getNumChannels();

        // Define the frequency of interest
        const float frequencyOfInterest = 1000.0f; // Example: 1000 Hz

        // Calculate the number of samples in one cycle of the frequency of interest
        const float sampleRate = 44100.f;
        const float samplesPerCycle = sampleRate / frequencyOfInterest;

        // Define the window size around the frequency of interest
        const int windowSize = static_cast<int>(samplesPerCycle * 2); // Example: Two cycles around the frequency

        // Initialize RMS accumulator
        float rmsAccumulator = 0.0f;

        // Iterate over each channel
        for (int channel = 0; channel < numChannels; ++channel)
        {
            // Iterate over the audio samples
            for (int i = 0; i < numSamples - windowSize; ++i)
            {
                // Calculate RMS within the window around the frequency of interest
                float sumSquared = 0.0f;
                for (int j = i; j < i + windowSize; ++j)
                {
                    float sample = tempBuffer.getSample(channel, j);
                    sumSquared += sample * sample;
                }

                float rms = std::sqrt(sumSquared / windowSize);
                rmsAccumulator += rms;
            }
        }

        // Calculate the average RMS across channels and samples
        float averageRMS = rmsAccumulator / (numChannels * (numSamples - windowSize));

        DBG(juce::Decibels::gainToDecibels(averageRMS));
    }
    elapsedTime = 0;
    stopTimer();

    signalThreadShouldExit();
}

void FrequencyLevelThread::updateBuffer(juce::AudioBuffer<float> newBuffer) {
    buffer = newBuffer;
}