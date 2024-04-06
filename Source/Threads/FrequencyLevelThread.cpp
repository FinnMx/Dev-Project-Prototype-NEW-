/*
  ==============================================================================

    FrequencyLevelThread.cpp
    Created: 6 Apr 2024 3:20:39pm
    Author:  New Owner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FrequencyLevelThread.h"

//==============================================================================
FrequencyLevelThread::FrequencyLevelThread() : juce::Thread("Frequency Level Thread"), fft(fftOrder), window(fftSize, juce::dsp::WindowingFunction<float>::hann)
{

}

FrequencyLevelThread::~FrequencyLevelThread()
{
}

void FrequencyLevelThread::timerCallback() {
    secondsElapsed++;
}

void FrequencyLevelThread::updateBuffer(juce::AudioBuffer<float> newBuffer) {
    buffer = newBuffer;
}

void FrequencyLevelThread::run() {
    processValues();

    signalThreadShouldExit();
}

void FrequencyLevelThread::processValues() {
    startTimer(1000);

    while (secondsElapsed >= 5) {
        auto currentBuffer = buffer;
        auto numSamples = currentBuffer.getNumSamples();

        // Apply windowing function
        window.multiplyWithWindowingTable(currentBuffer.getWritePointer(0), numSamples);

        // Perform FFT
        juce::dsp::AudioBlock<float> block(currentBuffer);
        fft.performFrequencyOnlyForwardTransform(block.getChannelPointer(0));

        // Get magnitude at desired frequency bin (e.g., 1000 Hz)
        float frequencyLevel = block.getSample(0, 1000 / (44100.f / fftSize));
        DBG(frequencyLevel);
    }
    
    stopTimer();
    secondsElapsed = 0;
}

