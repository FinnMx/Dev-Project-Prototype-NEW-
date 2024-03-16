/*
  ==============================================================================

    AudioSettingsComponent.h
    Created: 4 Mar 2024 4:11:48pm
    Author:  New Owner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AudioSettingsComponent  : public juce::Component,
                                public juce::Button::Listener
{
public:
    AudioSettingsComponent();
    ~AudioSettingsComponent() override;

    void buttonClicked(juce::Button* button) override;
    void setAudioSettings(juce::AudioDeviceSelectorComponent* newAudioSettings);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::AudioDeviceSelectorComponent* audioSettings;

    juce::TextButton apply;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioSettingsComponent)
};
