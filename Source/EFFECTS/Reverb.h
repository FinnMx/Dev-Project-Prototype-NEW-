/*
  ==============================================================================

    Reverb.h
    Created: 21 Feb 2024 8:40:11pm
    Author:  New Owner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Reverb  : public juce::AudioSource
{
public:
    Reverb();
    ~Reverb() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Reverb)
};
