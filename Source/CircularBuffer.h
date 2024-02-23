/*
  ==============================================================================

    CircularBuffer.h
    Created: 22 Feb 2024 11:01:45pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CircularBuffer  : public juce::AudioSource
{
public:
    CircularBuffer();
    ~CircularBuffer() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void setDelayGain(float newGain);
    void setDelayStatus(bool newStatus);

private:
    void fillBuffer(int channel, int bufferSize, int delayBufferSize, float* channelData);
    void readFromBuffer(int channel, int bufferSize, int delayBufferSize, const juce::AudioSourceChannelInfo& bufferToFill, juce::AudioBuffer<float>& delayBuffer);

    bool delayStatus{ false };
    float delayGain{ 0.f };

    int totalNumInputChannels{ 2 }, totalNumOutputChannels{ 2 };
    juce::AudioBuffer<float> delayBuffer;
    int writePosition{ 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CircularBuffer)
};
