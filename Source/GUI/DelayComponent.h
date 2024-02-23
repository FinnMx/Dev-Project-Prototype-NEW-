/*
  ==============================================================================

    DelayComponent.h
    Created: 15 Feb 2024 9:33:24pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <CircularBuffer.h>

//==============================================================================
/*
*/
class DelayComponent  : public juce::Component,
                        public juce::Slider::Listener
{
public:
    DelayComponent(CircularBuffer* circularBuffer);
    ~DelayComponent() override;

    void initSlider();
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragStarted(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    CircularBuffer* circularBuffer;

    juce::Slider gainSlider;

    juce::Label gainSliderLabel{ "Gain", "Gain" };
    juce::Label val{ "currentSliderVal", "" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayComponent)
};
