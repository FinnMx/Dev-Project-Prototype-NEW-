/*
  ==============================================================================

    ReverbComponent.h
    Created: 15 Feb 2024 9:33:35pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <Audio/TrackAudioPlayer.h>

//==============================================================================
/*
*/
class ReverbComponent  : public juce::Component,
                         public juce::Slider::Listener
{
public:
    ReverbComponent(TrackAudioPlayer* track1, TrackAudioPlayer* track2);
    ~ReverbComponent() override;

    void handleMidi(int control, int value = 0);

    void initSlider();
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragStarted(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;

    void setFocus(bool newFocus);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    bool isFocused{ false };

    TrackAudioPlayer* source1;
    TrackAudioPlayer* source2;

    juce::Slider roomSizeSlider;
    juce::Slider dampingSlider;
    juce::Slider wetLevelSlider;

    juce::Label roomSizeLabel{"Room Size", "Room Size"};
    juce::Label dampingLabel{ "Damping", "Damping" };
    juce::Label WetLevelLabel{ "Amount", "Amount" };
    juce::Label val{ "currentSliderVal", ""};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbComponent)
};
