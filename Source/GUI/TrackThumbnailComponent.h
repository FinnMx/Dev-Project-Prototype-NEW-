/*
  ==============================================================================

    TrackThumbnailComponent.h
    Created: 15 Feb 2024 9:35:52pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <Audio/TrackAudioPlayer.h>

//==============================================================================
/*
*/
class TrackThumbnailComponent : public juce::Component,
                                public juce::ChangeListener,
                                public juce::Slider::Listener,
                                public juce::Timer
{
public:
    TrackThumbnailComponent(std::string id, juce::AudioThumbnailCache& cache, juce::AudioFormatManager& formatManager, TrackAudioPlayer* audioPlayer);
    ~TrackThumbnailComponent() override;


    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragStarted(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;
    void timerCallback() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void loadFile(juce::File file);

    void initTimeSliderSlider();

private:
    juce::Rectangle<int> backDrop;

    juce::AudioThumbnail thumbnail;
    bool isLoaded;
    std::string id;

    TrackAudioPlayer* player;
    juce::Slider timeSlider;
    juce::File currentFile;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackThumbnailComponent)
};
