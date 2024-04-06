/*
  ==============================================================================

    FrequencyLevelThread.h
    Created: 6 Apr 2024 3:20:39pm
    Author:  New Owner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FrequencyLevelThread  : public juce::Thread, 
                              public juce::Timer
{
public:
    FrequencyLevelThread();
    ~FrequencyLevelThread() override;

    void updateBuffer(juce::AudioBuffer<float> newBuffer);

    void run() override;
    void timerCallback() override;

    void processValues();

private:

    const int fftOrder = 10; // FFT size = 2^fftOrder
    const int fftSize = 1 << fftOrder;

    juce::AudioBuffer<float> buffer;

    juce::dsp::FFT fft;
    juce::dsp::WindowingFunction<float> window;

    int secondsElapsed{ 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencyLevelThread)
};
