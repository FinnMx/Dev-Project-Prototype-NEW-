/*
  ==============================================================================

    DubSirenComponent.h
    Created: 28 Feb 2024 4:33:17pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DubSirenComponent  : public juce::Component
{
public:
    DubSirenComponent();
    ~DubSirenComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DubSirenComponent)
};
