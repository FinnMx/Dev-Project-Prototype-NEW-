/*
  ==============================================================================

    KillEQComponent.h
    Created: 15 Feb 2024 9:33:07pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <GUI/AudioVisualiserComponent.h>
#include <Audio/FrequencyCutoffs.h>

//==============================================================================
/*
*/
class KillEQComponent : public juce::AudioAppComponent,
    public juce::Slider::Listener,
    public juce::Button::Listener
{
public:
    KillEQComponent(FrequencyCutoffs* freqCutoffs);
    ~KillEQComponent() override;

    void initSlider();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //GET RID OF THIS BEFORE RELEASE
    void handleMidi(int action);

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    FrequencyCutoffs* freqCutoffs;

    juce::Slider subBassFrequencySlider;
    juce::Slider bassFrequencySlider;
    juce::Slider midsFrequencySlider;
    juce::Slider highFrequencySlider;

    juce::ToggleButton subBassOnOff;
    juce::ToggleButton bassOnOff;
    juce::ToggleButton midsOnOff;
    juce::ToggleButton highOnOff;

    juce::Label subBassFrequencyLabel{ "SubBassFrequencyr", "SubBassFrequency" };
    juce::Label bassFrequencyLabel{ "bassFrequency", "bassFrequency" };
    juce::Label midsFrequencyLabel{ "midsFrequency", "midsFrequency" };
    juce::Label highFrequencyLabel{ "highFrequency", "highFrequency" };

    AudioVisualiserComponent visualiser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KillEQComponent)
};