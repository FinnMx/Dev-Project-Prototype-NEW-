/*
  ==============================================================================

    MasterComponent.h
    Created: 15 Feb 2024 9:34:35pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <GUI/MeterComponent.h>
#include <Audio/ExternalInput.h>

//==============================================================================
/*
*/
class MasterComponent  : public juce::Component,
                         public juce::Timer,
                         public juce::Button::Listener
{
public:
    MasterComponent(float* rmsMasterLeft,float* rmsMasterRight, juce::MixerAudioSource* mixerSource, ExternalInput* externalInput);
    ~MasterComponent() override;

    void timerCallback() override;

    void buttonClicked(juce::Button* button) override;
    void toggleExternalInput();

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    MeterComponent rmsLeft;
    MeterComponent rmsRight;
    juce::Label VolumeLabel;

    juce::MixerAudioSource* mixerSource;
    ExternalInput* externalInput;

    juce::ToggleButton inputToggler;

    float* masterLeft;
    float* masterRight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterComponent)
};
