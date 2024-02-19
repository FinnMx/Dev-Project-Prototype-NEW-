/*
  ==============================================================================

    TrackThumbnailComponent.h
    Created: 15 Feb 2024 9:35:52pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <TrackAudioPlayer.h>

//==============================================================================
/*
*/
class TrackThumbnailComponent : public juce::Component,
    public juce::ChangeListener
{
public:
    TrackThumbnailComponent(std::string id, juce::AudioThumbnailCache& cache, juce::AudioFormatManager& formatManager);
    ~TrackThumbnailComponent() override;


    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void paint(juce::Graphics&) override;
    void resized() override;

    void loadFile(juce::File file);

private:
    juce::Rectangle<int> backDrop;

    juce::AudioThumbnail thumbnail;
    bool isLoaded;
    std::string id;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackThumbnailComponent)
};
