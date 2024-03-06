/*
  ==============================================================================

    InputComponent.h
    Created: 15 Feb 2024 9:33:49pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <Audio/TrackAudioPlayer.h>
#include <GUI/MeterComponent.h>
#include <GUI/TrackThumbnailComponent.h>

//==============================================================================
/*
*/
class InputComponent  : public juce::Component,
                        public juce::Slider::Listener,
                        public juce::Button::Listener,
                        public juce::Timer
{
public:
    InputComponent(std::string id, TrackAudioPlayer *audioPlayer, juce::AudioFormatManager &formatManager, TrackThumbnailComponent* trackThumb);
    ~InputComponent() override;

    void timerCallback() override;
    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    void initVolSlider();


    TrackAudioPlayer* player;
    TrackThumbnailComponent* trackThumbnail;

    MeterComponent MeterL, MeterR;

    std::string trackName;
    juce::TextButton trackLoad;
    juce::TextButton trackPlay;
    juce::TextButton trackPause;
    juce::TextButton trackStop;

    juce::Slider volSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputComponent)
};
