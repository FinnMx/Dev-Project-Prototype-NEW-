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
DubSirenComponent::DubSirenComponent(DubSiren* dubSiren) : dubSiren(dubSiren)
{
    initSlider();

    addAndMakeVisible(trigger);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(presetSlider);

    trigger.addListener(this);

    volumeSlider.addListener(this);
    presetSlider.addListener(this);
}

DubSirenComponent::~DubSirenComponent()
{
}

void DubSirenComponent::initSlider() {
    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    volumeSlider.setRange(0.f, +6.f, 0.1f);
    volumeSlider.setValue(0.5f);
    volumeSliderLabel.attachToComponent(&volumeSlider, false);
    volumeSliderLabel.setJustificationType(juce::Justification::centredBottom);

    presetSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    presetSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    presetSlider.setRange(1.f, +8.f, 1.f);
    presetSlider.setValue(1.f);
    presetSliderLabel.attachToComponent(&presetSlider, false);
    presetSliderLabel.setJustificationType(juce::Justification::centredBottom);
}

void DubSirenComponent::buttonClicked(juce::Button* button) {
    if (button = &trigger)
        dubSiren->setTrigger(button->getToggleState());
}

void DubSirenComponent::sliderValueChanged(juce::Slider* slider) {
    switch ((int)presetSlider.getValue()) {
    case 1:
        dubSiren->setWaveType(1);
        dubSiren->setLFOWaveType(3);
        dubSiren->setFrequency(300.f);
        dubSiren->setLfoFrequency(5.f);
        break;
    case 2:
        dubSiren->setWaveType(2);
        dubSiren->setLFOWaveType(5);
        dubSiren->setFrequency(250.f);
        dubSiren->setLfoFrequency(8.f);
        break;
    case 3:
        dubSiren->setWaveType(1);
        dubSiren->setLFOWaveType(4);
        dubSiren->setFrequency(130.81f);
        dubSiren->setLfoFrequency(3.5f);
        break;
    case 4:
        dubSiren->setWaveType(1);
        dubSiren->setLFOWaveType(3);
        dubSiren->setFrequency(130.81f);
        dubSiren->setLfoFrequency(7.0f);
        break;
    case 5:
        dubSiren->setWaveType(1);
        dubSiren->setLFOWaveType(2);
        dubSiren->setFrequency(130.81f);
        dubSiren->setLfoFrequency(5.0f);
        break;
    case 6:
        dubSiren->setWaveType(1);
        dubSiren->setLFOWaveType(3);
        dubSiren->setFrequency(261.63f);
        dubSiren->setLfoFrequency(0.75f);
        break;
    case 7:
        dubSiren->setWaveType(1);
        dubSiren->setLFOWaveType(2);
        dubSiren->setFrequency(523.25f);
        dubSiren->setLfoFrequency(0.25f);
        break;
    case 8:
        dubSiren->setWaveType(1);
        dubSiren->setLFOWaveType(1);
        dubSiren->setFrequency(523.25f);
        dubSiren->setLfoFrequency(0.25f);
        break;
    }
    dubSiren->setVolume(volumeSlider.getValue());
    val.setText(std::to_string(slider->getValue()), juce::NotificationType{});
}

void DubSirenComponent::sliderDragStarted(juce::Slider* slider) {
    val.attachToComponent(slider, juce::Justification::centredLeft);
}

void DubSirenComponent::sliderDragEnded(juce::Slider* slider) {
    val.setText("", juce::NotificationType{});
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

void DubSirenComponent::resized()
{
    /*
    frequencySlider.setBounds(getWidth() * 0.1,
        getHeight() * 0.25,
        getWidth() * 0.3,
        getHeight() * 0.4);

    
    lfoFrequencySlider.setBounds(getWidth() * 0.6,
        getHeight() * 0.25,
        getWidth() * 0.3,
        getHeight() * 0.4);
    
    waveTypeSlider.setBounds(getWidth() * 0.6,
        getHeight() * 0.6,
        getWidth() * 0.3,
        getHeight() * 0.3);

    */
    presetSlider.setBounds(getWidth() * 0.1,
        getHeight() * 0.25,
        getWidth() * 0.3,
        getHeight() * 0.4);

    volumeSlider.setBounds(getWidth() * 0.6,
        getHeight() * 0.65,
        getWidth() * 0.3,
        getHeight() * 0.4);

    trigger.setBounds(getWidth() * 0.2,
        getHeight() * 0.8,
        getWidth() * 0.2,
        getHeight() * 0.2);
}
