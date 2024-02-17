/*
  ==============================================================================

    GUIComponent.h
    Created: 17 Feb 2024 8:11:19pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class GUIComponent  : public juce::Component
{
public:
    GUIComponent();
    ~GUIComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //graphics
    juce::Rectangle<int> inputPanel;
    juce::Rectangle<int> masterPanel;
    juce::Rectangle<int> thumbnailPanel;
    juce::Rectangle<int> tenBandPanel;
    juce::Rectangle<int> reverbPanel;
    juce::Rectangle<int> delayPanel;
    juce::Rectangle<int> killEQPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GUIComponent)
};
