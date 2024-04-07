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
FrequencyLevelThread::FrequencyLevelThread(float* frequencies, float(&averageRMSValues)[10]) : juce::Thread("Frequency Level Thread"),
frequencies(frequencies), averageRMSValues(averageRMSValues)
{
    buffer.clear();
    for (auto& smooth : smoother)
        smooth.reset(44100.f, 0.001f);
}

FrequencyLevelThread::~FrequencyLevelThread()
{
    signalThreadShouldExit();
}

void FrequencyLevelThread::timerCallback() {
    elapsedTime++;
    for (int i = 0; i < 10; i++) {
        averageRMSValues[i] = juce::Decibels::gainToDecibels(dBtodBFS(averageRMSValues[i] / iterations));
        DBG(averageRMSValues[i] );
        if (averageRMSValues[i] < targetRMSValues[i]) {
            smoother[i].setTargetValue(smoother[i].getCurrentValue() + (targetRMSValues[i] - averageRMSValues[i]) );
        } else if(averageRMSValues[i] > targetRMSValues[i]){
            smoother[i].setTargetValue(smoother[i].getCurrentValue() - (averageRMSValues[i] - targetRMSValues[i]));
        }
        averageRMSValues[i] = 0;
    }
    iterations = 0;
}

void FrequencyLevelThread::setSliders(std::vector<juce::Slider*>& sliders) {
    passedSliders = sliders;
}

void FrequencyLevelThread::setTargetRMSValues(float* newTargetRMSValues) {
    targetRMSValues = newTargetRMSValues;
}

float FrequencyLevelThread::dBtodBFS(float dB) {
    float amplitude = std::pow(10.f, dB / 20.f);

    // Convert linear scale amplitude to dBFS
    float maxAmplitude = 1.f; // Maximum amplitude (1 for normalized audio)
    float dBFS = 20.f * std::log10(amplitude / maxAmplitude);

    return dBFS;
}

void FrequencyLevelThread::run() {
    startTimer(1000);

    for(int i = 0; i < 10; i++)
        smoother[i].setValue(passedSliders[i]->getValue(), true);

    while(elapsedTime <= 5){
        for (int i = 0; i < 10; i++) {
            auto tempBuffer = buffer; // Input audio buffer
            const int numSamples = tempBuffer.getNumSamples();
            const int numChannels = tempBuffer.getNumChannels();

            // Define the frequency of interest
            const float frequencyOfInterest = frequencies[i];

            // Calculate the number of samples in one cycle of the frequency of interest
            const float sampleRate = 44100.f;
            float samplesPerCycle = sampleRate / frequencyOfInterest;

            if (samplesPerCycle > 512)
                samplesPerCycle = 256;

            // Define the window size around the frequency of interest
            const int windowSize = static_cast<int>(samplesPerCycle); // Example: Two cycles around the frequency

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
            averageRMSValues[i] += rmsAccumulator / (numChannels * (numSamples - windowSize));
            const juce::MessageManagerLock mmLock;
            passedSliders[i]->setValue(smoother[i].getNextValue());
        }
        iterations++;
    }
    elapsedTime = 0;
    iterations = 0;
    stopTimer();

    signalThreadShouldExit();
}

void FrequencyLevelThread::updateBuffer(juce::AudioBuffer<float> newBuffer) {
    buffer = newBuffer;
}