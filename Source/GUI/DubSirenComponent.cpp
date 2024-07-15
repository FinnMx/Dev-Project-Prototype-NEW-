/*
  ==============================================================================

    DubSirenComponent.cpp
    Created: 28 Feb 2024 4:33:17pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DubSirenComponent.h"

//==============================================================================
DubSirenComponent::DubSirenComponent(DubSiren* dubSiren, SettingsHandler* settingsHandler) : dubSiren(dubSiren), settingsHandler(settingsHandler)
{
    presets = settingsHandler->LoadDubSirenSettings();
    initSlider();

    addAndMakeVisible(trigger);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(lfoFrequencySlider);
    addAndMakeVisible(frequencySlider);
    addAndMakeVisible(waveTypeSlider);
    addAndMakeVisible(lfoWaveTypeSlider);
    //addAndMakeVisible(presetSlider);

    addAndMakeVisible(nextPreset);
    addAndMakeVisible(prevPreset);

    trigger.addListener(this);
    nextPreset.addListener(this);
    prevPreset.addListener(this);

    lfoWaveTypeSlider.addListener(this);
    lfoFrequencySlider.addListener(this);
    frequencySlider.addListener(this);
    waveTypeSlider.addListener(this);
    volumeSlider.addListener(this);
}

DubSirenComponent::~DubSirenComponent()
{
    savePreset();

    settingsHandler->SaveDubSirenSettings(presets);
}

void DubSirenComponent::initSlider() {
    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    volumeSlider.setRange(0.f, +6.f, 0.1f);
    volumeSlider.setValue(0.5f);
    volumeSliderLabel.attachToComponent(&volumeSlider, false);
    volumeSliderLabel.setJustificationType(juce::Justification::centredBottom);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 25);

    lfoFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    lfoFrequencySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    lfoFrequencySlider.setRange(0.f, +50.f, 0.1f);
    lfoFrequencySlider.setValue(presets[0][1]);
    lfoFrequencySliderLabel.attachToComponent(&lfoFrequencySlider, false);
    lfoFrequencySliderLabel.setJustificationType(juce::Justification::centredBottom);
    lfoFrequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 25);

    frequencySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    frequencySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    frequencySlider.setRange(100.f, +1000.f, 1.f);
    frequencySlider.setValue(presets[0][0]);
    frequencySliderLabel.attachToComponent(&frequencySlider, false);
    frequencySliderLabel.setJustificationType(juce::Justification::centredBottom);
    frequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 25);

    waveTypeSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    waveTypeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    waveTypeSlider.setRange(1.f, +2.f, 1.f);
    waveTypeSlider.setValue(presets[0][2]);
    waveTypeSliderLabel.attachToComponent(&waveTypeSlider, false);
    waveTypeSliderLabel.setJustificationType(juce::Justification::centredBottom);
    waveTypeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 25);

    lfoWaveTypeSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    lfoWaveTypeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, NULL, NULL);
    lfoWaveTypeSlider.setRange(1.f, +5.f, 1.f);
    lfoWaveTypeSlider.setValue(presets[0][3]);
    lfoWaveTypeSliderLabel.attachToComponent(&lfoWaveTypeSlider, false);
    lfoWaveTypeSliderLabel.setJustificationType(juce::Justification::centredBottom);
    lfoWaveTypeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 25);

}

void DubSirenComponent::buttonClicked(juce::Button* button) {
    if (button == &trigger)
        dubSiren->setTrigger(button->getToggleState());

    if (button == &prevPreset) {
        savePreset();
        currentPreset--;
        if (currentPreset < 0)
            currentPreset = 4;
        applyPreset();
    }
    if (button == &nextPreset) {
        savePreset();
        currentPreset++;
        if (currentPreset > 4)
            currentPreset = 0;
        applyPreset();
    }
}

void DubSirenComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &volumeSlider)
        dubSiren->setVolume(volumeSlider.getValue());
    if (slider == &frequencySlider)
        dubSiren->setFrequency(frequencySlider.getValue());
    if (slider == &lfoFrequencySlider)
        dubSiren->setLfoFrequency(lfoFrequencySlider.getValue());
    if (slider == &waveTypeSlider)
        dubSiren->setWaveType(waveTypeSlider.getValue());
    if (slider == &lfoWaveTypeSlider)
        dubSiren->setLFOWaveType(lfoWaveTypeSlider.getValue());
    val.setText(std::to_string(slider->getValue()), juce::NotificationType{});
}

void DubSirenComponent::sliderDragStarted(juce::Slider* slider) {
}

void DubSirenComponent::sliderDragEnded(juce::Slider* slider) {
}

void DubSirenComponent::handleMidi(int action, int value) {
    const juce::MessageManagerLock mmLock;
    switch (action) {
    case 0:
        presetSlider.setValue(juce::jmap((float)value, (float)0, (float)127, 1.f, 8.f));
        break;
    case 1:
        trigger.triggerClick();
        break;
    case 2:
        volumeSlider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.f, 6.f));
        break;
    }
}

void DubSirenComponent::setFocus(bool newFocus) {
    const juce::MessageManagerLock mmLock;
    isFocused = newFocus;
    repaint();
}

void DubSirenComponent::paint (juce::Graphics& g)
{
    if (isFocused)
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).withAlpha(.1f));   // clear the background
    if (!isFocused)
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(getLookAndFeel().findColour(juce::TextEditor::outlineColourId).contrasting(0.15f));
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(30.0f);
    g.drawText("Dub Siren", getLocalBounds(),
        juce::Justification::centredTop, true);
}


void DubSirenComponent::applyPreset() {
    frequencySlider.setValue(presets[currentPreset][0]);
    lfoFrequencySlider.setValue(presets[currentPreset][1]);
    waveTypeSlider.setValue(presets[currentPreset][2]);
    lfoWaveTypeSlider.setValue(presets[currentPreset][3]);
}

void DubSirenComponent::savePreset() {
    presets[currentPreset][0] = frequencySlider.getValue();
    presets[currentPreset][1] = lfoFrequencySlider.getValue();
    presets[currentPreset][2] = waveTypeSlider.getValue();
    presets[currentPreset][3] = lfoWaveTypeSlider.getValue();
}


void DubSirenComponent::resized()
{
    
    frequencySlider.setBounds(getWidth() * 0.1,
        getHeight() * 0.15,
        getWidth() * 0.2,
        getHeight() * 0.3);

    lfoFrequencySlider.setBounds(getWidth() * 0.5,
        getHeight() * 0.15,
        getWidth() * 0.2,
        getHeight() * 0.3);
    
    waveTypeSlider.setBounds(getWidth() * 0.1,
        getHeight() * 0.55,
        getWidth() * 0.2,
        getHeight() * 0.3);

    lfoWaveTypeSlider.setBounds(getWidth() * 0.5,
        getHeight() * 0.55,
        getWidth() * 0.2,
        getHeight() * 0.3);

    //

    /*
    presetSlider.setBounds(getWidth() * 0.1,
        getHeight() * 0.25,
        getWidth() * 0.3,
        getHeight() * 0.4);
        */

    volumeSlider.setBounds(getWidth() * 0.8,
        getHeight() * 0.15,
        getWidth() * 0.2,
        getHeight() * 0.6);

    trigger.setBounds(getWidth() * 0.9,
        getHeight() * 0.825,
        getWidth() * 0.2,
        getHeight() * 0.2);

    prevPreset.setBounds(getWidth() * 0.125,
        getHeight() * 0.9,
        getWidth() * 0.05,
        getHeight() * 0.05);

    nextPreset.setBounds(getWidth() * 0.2,
        getHeight() * 0.9,
        getWidth() * 0.05,
        getHeight() * 0.05);

}
