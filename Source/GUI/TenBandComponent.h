/*
  ==============================================================================

    TenBandComponent.h
    Created: 15 Feb 2024 9:38:04pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TenBandComponent  : public juce::Component
{
public:
    TenBandComponent();
    ~TenBandComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TenBandComponent)
};
