/*
  ==============================================================================

    InputComponent.h
    Created: 15 Feb 2024 9:33:49pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <TrackAudioPlayer.h>

//==============================================================================
/*
*/
class InputComponent  : public juce::Component,
                        public juce::Slider::Listener,
                        public juce::Button::Listener
{
public:
    InputComponent(std::string id, TrackAudioPlayer *audioPlayer, juce::AudioFormatManager &formatManager);
    ~InputComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    TrackAudioPlayer* player;

    std::string trackName;
    juce::TextButton trackLoad;
    juce::TextButton trackPlay;
    juce::TextButton trackPause;
    juce::TextButton trackStop;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputComponent)
};
