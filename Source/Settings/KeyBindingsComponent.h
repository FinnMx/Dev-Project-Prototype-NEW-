/*
  ==============================================================================

    KeyBindingsComponent.h
    Created: 4 Mar 2024 4:11:59pm
    Author:  New Owner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class KeyBindingsComponent  : public juce::Component
{
public:
    KeyBindingsComponent();
    ~KeyBindingsComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    std::string headings[8] = { "Input A Bindings", "Input B Bindings", "Thumbnail Bindings", "10 Band Bindings","Dub Siren Bindings", "Reverb Bindings", "Delay Bindings", "Kill EQ Bindings" };
    juce::Label headingsLabels[8];
    float headingLabelsX[8] = { 0.005, 0.005, 0.005, 0.355, 0.355, 0.705, 0.705, 0.705 };
    float headingLabelsY[8] = { 0.005, 0.355, 0.655, 0.005, 0.555, 0.005, 0.355, 0.655 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyBindingsComponent)
};
