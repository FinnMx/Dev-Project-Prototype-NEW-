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
#include <GUI/AudioVisualiserComponent.h>

//==============================================================================
/*
*/
class DelayComponent  : public juce::AudioAppComponent,
                        public juce::Slider::Listener,
                        public juce::Button::Listener
{
public:
    DelayComponent(CircularBuffer* circularBuffer);
    ~DelayComponent() override;

    void initSlider();

    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragStarted(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void handleMidi(int control, int value = 0);

    void buttonClicked(juce::Button* button) override;

    void setFocus(bool newFocus);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    bool isFocused{ true };

    CircularBuffer* circularBuffer;
    AudioVisualiserComponent visualiser;

    juce::Slider gainSlider; // FEEDBACK!!!!! NOT GAIn!!!
    juce::Slider timeSlider;
    juce::Slider frequencyCutSlider;

    juce::ToggleButton onOff;

    juce::Label gainSliderLabel{ "Feedback", "Feedback" };
    juce::Label timeSliderLabel{ "Time", "Time" };
    juce::Label frequencyCutSliderLabel{ "Frequency", "Frequency" };
    juce::Label val{ "currentSliderVal", "" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayComponent)
};
