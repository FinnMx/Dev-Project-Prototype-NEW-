/*
  ==============================================================================

    TenBandComponent.h
    Created: 15 Feb 2024 9:38:04pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <Audio/TenBandEQ.h>

//==============================================================================
/*
*/
class TenBandComponent  : public juce::Component,
                          public juce::Slider::Listener,
                          public juce::Button::Listener
{
public:
    TenBandComponent(TenBandEQ* tenBandEQ);
    ~TenBandComponent() override;

    void handleMidi(int action, int value = 0);

    void initSlider();
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    TenBandEQ* tenBand;

    std::vector<juce::Slider*> sliders;
    std::vector<juce::Label*> sliderLabels;

    juce::TextButton resetButton{ "reset" };
    juce::TextButton autoSetButton{ "auto set" };

    juce::Slider freq30Slider;
    juce::Slider freq62Slider;
    juce::Slider freq125Slider;
    juce::Slider freq250Slider;
    juce::Slider freq500Slider;
    juce::Slider freq1000Slider;
    juce::Slider freq2000Slider;
    juce::Slider freq4000Slider;
    juce::Slider freq8000Slider;
    juce::Slider freq16000Slider;

    juce::Label freq30SliderLabel{ "30Hz", "30Hz" };
    juce::Label freq62SliderLabel{ "62Hz", "62Hz" };
    juce::Label freq125SliderLabel{ "125Hz", "125Hz" };
    juce::Label freq250SliderLabel{ "250Hz", "250Hz" };
    juce::Label freq500SliderLabel{ "500Hz", "500Hz" };
    juce::Label freq1000SliderLabel{ "1KHz", "1KHz" };
    juce::Label freq2000SliderLabel{ "2KHz", "2KHz" };
    juce::Label freq4000SliderLabel{ "4KHz", "4KHz" };
    juce::Label freq8000SliderLabel{ "8KHz", "8KHz" };
    juce::Label freq16000SliderLabel{ "16KHz", "16KHz" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TenBandComponent)
};
