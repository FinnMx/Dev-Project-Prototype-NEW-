/*
  ==============================================================================

    KillEQComponent.h
    Created: 15 Feb 2024 9:33:07pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <GUI/AudioVisualiserComponent.h>

//==============================================================================
/*
*/
class KillEQComponent  : public juce::AudioAppComponent
{
public:
    KillEQComponent();
    ~KillEQComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    AudioVisualiserComponent demo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KillEQComponent)
};
