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
    void setFrequency(float newFrequency);
    void setLfoFrequency(float newLfoFrequency);
    void setModulationDepth(float newModulationDepth);
    void setVolume(float newVolume);

    void updateAngleDelta();

private:
    juce::AudioBuffer<float> copyBuffer;
    bool trigger{ false };

    float frequency{ 340 };
    float lfoFrequency{ 0.5 };
    float modulationDepth{ 60.0 };
    float volume{ 0.5f };

    juce::dsp::Oscillator<float> oscillator;
    juce::dsp::Oscillator<float> lfoOscillator;

    juce::IIRFilter filter;

    int totalNumInputChannels{ 2 }, totalNumOutputChannels{ 2 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DubSiren)
};
