/*
  ==============================================================================

    PopoutWindow.h
    Created: 6 Mar 2024 12:46:47pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PopoutWindow  : public juce::DocumentWindow
{
public:
    PopoutWindow(const juce::String& name, juce::Component* component, int x, int y);
    ~PopoutWindow() override;

    void closeButtonPressed() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Component* component;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PopoutWindow)
};
