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
    std::unique_ptr<juce::Label> bindLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyBindingsComponent)
};
