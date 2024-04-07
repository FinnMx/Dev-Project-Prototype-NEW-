/*
  ==============================================================================

    FrequencyLevelThread.h
    Created: 6 Apr 2024 9:56:06pm
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
    FrequencyLevelThread(float* frequencies, float (&averageRMSValues)[10]);
    ~FrequencyLevelThread() override;

    void run() override;
    void timerCallback() override;

    void setSliders(std::vector<juce::Slider*>& sliders);

    void updateBuffer(juce::AudioBuffer<float> newBuffer);

private:
    float dBtodBFS(float dB);

    juce::AudioBuffer<float> buffer;

    std::vector<juce::Slider*> passedSliders;

    float* averageRMSValues;
    float* frequencies;
    int elapsedTime{ 0 };
    int iterations{ 0 };
    float test[10] = { 0,0,0,0,0,0,0,0,0,0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencyLevelThread)
};
