/*
  ==============================================================================

    TrackThumbnailComponent.h
    Created: 15 Feb 2024 9:35:52pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TrackThumbnailComponent  : public juce::Component
{
public:
    TrackThumbnailComponent();
    ~TrackThumbnailComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackThumbnailComponent)
};
