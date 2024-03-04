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
class AudioSettingsComponent  : public juce::Component
{
public:
    AudioSettingsComponent();
    ~AudioSettingsComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioSettingsComponent)
};
