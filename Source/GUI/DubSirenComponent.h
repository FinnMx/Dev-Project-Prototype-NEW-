/*
  ==============================================================================

    DubSirenComponent.h
    Created: 28 Feb 2024 4:33:17pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <DubSiren.h>

//==============================================================================
/*
*/
class DubSirenComponent  : public juce::Component,
                           public juce::Button::Listener
{
public:
    DubSirenComponent(DubSiren* dubSiren);
    ~DubSirenComponent() override;

    void buttonClicked(juce::Button* button) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    DubSiren* dubSiren;

    juce::ToggleButton trigger;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DubSirenComponent)
};
