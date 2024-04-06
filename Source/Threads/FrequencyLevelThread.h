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
    FrequencyLevelThread(float* frequencies);
    ~FrequencyLevelThread() override;

    void run() override;
    void timerCallback() override;

    void updateBuffer(juce::AudioBuffer<float> newBuffer);

private:
    juce::AudioBuffer<float> buffer;

    float* frequencies;
    int elapsedTime{ 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencyLevelThread)
};
