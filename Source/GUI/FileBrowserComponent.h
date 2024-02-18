/*
  ==============================================================================

    FileBrowserComponent.h
    Created: 12 Feb 2024 1:50:50pm
    Author:  New Owner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FileBrowserComponent  : public juce::Component 
{
public:
    FileBrowserComponent();
    ~FileBrowserComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::FileBrowserComponent fileBrowser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileBrowserComponent)
};
