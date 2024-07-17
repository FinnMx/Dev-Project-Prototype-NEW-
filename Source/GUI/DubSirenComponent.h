/*
  ==============================================================================

    DubSirenComponent.h
    Created: 28 Feb 2024 4:33:17pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <Audio/DubSiren.h>
#include <Settings/SettingsHandler.h>

//==============================================================================
/*
*/
class DubSirenComponent  : public juce::Component,
                           public juce::Button::Listener,
                           public juce::Slider::Listener
{
public:
    DubSirenComponent(DubSiren* dubSiren, SettingsHandler* settingsHandler);
    ~DubSirenComponent() override;

    void initSlider();

    void handleMidi(int action, int value = 0);

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragStarted(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;

    void setFocus(bool newFocus);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    DubSiren* dubSiren;
    SettingsHandler* settingsHandler;

    void applyPreset();
    void savePreset();

    bool isFocused { false };

    std::vector<std::array<float,4>> presets;
    int currentPreset{ 0 };
    juce::TextButton nextPreset{">"};
    juce::TextButton prevPreset{"<"};


    juce::Slider frequencySlider;
    juce::Slider lfoFrequencySlider;
    juce::Slider volumeSlider;
    juce::Slider waveTypeSlider;
    juce::Slider lfoWaveTypeSlider;

    juce::Slider presetSlider;

    juce::ToggleButton trigger;

    juce::Label currentPresetLabel{ "currentPresetLabel", "currentPresetLabel" };

    juce::Label frequencySliderLabel{ "Frequency", "Frequency" };
    juce::Label lfoFrequencySliderLabel{ "LfoFrequency", "LfoFrequency" };
    juce::Label waveTypeSliderLabel{ "waveType", "waveType" };
    juce::Label volumeSliderLabel{ "Volume", "Volume" };
    juce::Label lfoWaveTypeSliderLabel{ "lfoWaveType", "lfoWaveType" };

    juce::Label val{ "currentSliderVal", "" };

    juce::Label presetSliderLabel{ "Peset", "Preset" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DubSirenComponent)
};
