/*
  ==============================================================================

    ReverbComponent.h
    Created: 15 Feb 2024 9:33:35pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ReverbComponent  : public juce::AudioAppComponent
{
public:
    ReverbComponent(juce::ResamplingAudioSource* resampleSource);
    ~ReverbComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void initSlider();

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ResamplingAudioSource* source;
    juce::ReverbAudioSource reverbSource{ source, false }; // maybe source should be ptr?
    juce::Reverb::Parameters parameters;

    juce::Slider roomSizeSlider;
    juce::Slider dampingSlider;
    juce::Slider wetLevelSlider;
    juce::Slider dryLevelSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbComponent)
};
