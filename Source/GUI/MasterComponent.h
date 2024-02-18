/*
  ==============================================================================

    MasterComponent.h
    Created: 15 Feb 2024 9:34:35pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MasterComponent  : public juce::Component
{
public:
    MasterComponent();
    ~MasterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterComponent)
};
