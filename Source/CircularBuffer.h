/*
  ==============================================================================

    CircularBuffer.h
    Created: 22 Feb 2024 11:01:45pm
    Author:  Finn

  ==============================================================================
*/

#pragma once
#include <TrackAudioPlayer.h>
#include <JuceHeader.h>

//==============================================================================
/*
*/
class CircularBuffer : public juce::AudioSource
{
public:
    CircularBuffer(TrackAudioPlayer* track1, TrackAudioPlayer* track2);
    ~CircularBuffer() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void setDelayFeedback(float newFeedback);
    void setDelayStatus(bool newStatus);
    void setDelayTime(float newTime);

private:
    void fillBuffer(int channel, int bufferSize, int delayBufferSize, float* channelData);
    void readFromBuffer(int channel, int bufferSize, int delayBufferSize, const juce::AudioSourceChannelInfo& bufferToFill, juce::AudioBuffer<float>& delayBuffer);

    bool delayStatus{ false };
    float delayFeedback{ 0.f };
    float delayTime{ 1.f };
    float rampingVal{ 0.f };

    int totalNumInputChannels{ 2 }, totalNumOutputChannels{ 2 };
    /*
    juce::AudioBuffer<float> delayBuffer;
    int writePosition{ 0 };
    int positionDifference{ 0 };
    */

    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delay{132300};
    juce::LinearSmoothedValue<float> smoothedTime;

    TrackAudioPlayer* source1;
    TrackAudioPlayer* source2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CircularBuffer)
};
