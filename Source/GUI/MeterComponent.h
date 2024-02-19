/*
  ==============================================================================

    MeterComponent.h
    Created: 18 Feb 2024 2:16:33pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MeterComponent  : public juce::Component
{
public:
    MeterComponent();
    ~MeterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setLevel(const float val)  ;

private:
    float level = -60.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterComponent)
};
