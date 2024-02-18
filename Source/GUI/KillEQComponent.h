/*
  ==============================================================================

    KillEQComponent.h
    Created: 15 Feb 2024 9:33:07pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class KillEQComponent  : public juce::Component
{
public:
    KillEQComponent();
    ~KillEQComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KillEQComponent)
};
