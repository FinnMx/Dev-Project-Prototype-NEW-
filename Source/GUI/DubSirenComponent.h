/*
  ==============================================================================

    DubSirenComponent.h
    Created: 28 Feb 2024 4:33:17pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <DubSiren.h>

//==============================================================================
/*
*/
class DubSirenComponent  : public juce::Component,
                           public juce::Button::Listener,
                           public juce::Slider::Listener
{
public:
    DubSirenComponent(DubSiren* dubSiren);
    ~DubSirenComponent() override;

    void initSlider();

    void handleMidi(int control, int value = 0);

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragStarted(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;

    void setFocus(bool newFocus);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    DubSiren* dubSiren;

    bool isFocused { false };

    juce::Slider frequencySlider;
    juce::Slider lfoFrequencySlider;
    juce::Slider volumeSlider;

    juce::ToggleButton trigger;

    juce::Label frequencySliderLabel{ "Frequency", "Frequency" };
    juce::Label lfoFrequencySliderLabel{ "LfoFrequency", "LfoFrequency" };
    juce::Label volumeSliderLabel{ "Volume", "Volume" };
    juce::Label val{ "currentSliderVal", "" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DubSirenComponent)
};
