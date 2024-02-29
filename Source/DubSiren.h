/*
  ==============================================================================

    DubSiren.h
    Created: 29 Feb 2024 3:10:56pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DubSiren  : public juce::AudioSource
{
public:
    DubSiren();
    ~DubSiren() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void setTrigger(bool newTrigger);

    void updateAngleDelta();

private:
    juce::AudioBuffer<float> copyBuffer;
    bool trigger{ false };

    float frequency{ 100 };
    juce::dsp::Oscillator<float> oscillator;
    juce::dsp::Oscillator<float> lfoOscillator;

    int totalNumInputChannels{ 2 }, totalNumOutputChannels{ 2 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DubSiren)
};
