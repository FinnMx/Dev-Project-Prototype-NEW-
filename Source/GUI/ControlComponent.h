/*
  ==============================================================================

    ControlComponent.h
    Created: 12 Feb 2024 3:38:14pm
    Author:  New Owner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ControlComponent  : public juce::Component
{
public:
    ControlComponent();
    ~ControlComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextButton playButton;
    juce::TextButton pauseButton;
    juce::TextButton stopButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlComponent)
};
