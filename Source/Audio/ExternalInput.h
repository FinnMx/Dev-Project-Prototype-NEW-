/*
  ==============================================================================

    ExternalInput.h
    Created: 14 Mar 2024 2:51:32pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <random>

//==============================================================================
/*
*/
class ExternalInput  : public juce::AudioSource
{
public:
    ExternalInput(juce::AudioDeviceManager* deviceManager);
    ~ExternalInput() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;


private:
    juce::AudioDeviceManager* deviceManager;
    juce::Random random;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExternalInput)
};
