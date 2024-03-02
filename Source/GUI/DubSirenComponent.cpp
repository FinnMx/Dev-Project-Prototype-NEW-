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
    addAndMakeVisible(frequencySlider);
    addAndMakeVisible(lfoFrequencySlider);
    addAndMakeVisible(volumeSlider);

    trigger.addListener(this);

    frequencySlider.addListener(this);
    lfoFrequencySlider.addListener(this);
    volumeSlider.addListener(this);
}

DubSirenComponent::~DubSirenComponent()
{
}

void DubSirenComponent::initSlider() {
    frequencySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    frequencySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    frequencySlider.setRange(30.f, +1000.0f, 0.01f);
    frequencySlider.setValue(30.f);
    frequencySliderLabel.attachToComponent(&frequencySlider, false);
    frequencySliderLabel.setJustificationType(juce::Justification::centredBottom);

    lfoFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    lfoFrequencySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    lfoFrequencySlider.setRange(0.f, +2.0f, 0.01f);
    lfoFrequencySlider.setValue(0.5f);
    lfoFrequencySliderLabel.attachToComponent(&lfoFrequencySlider, false);
    lfoFrequencySliderLabel.setJustificationType(juce::Justification::centredBottom);

    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    volumeSlider.setRange(0.f, +6.f, 0.1f);
    volumeSlider.setValue(0.5f);
    volumeSliderLabel.attachToComponent(&volumeSlider, false);
    volumeSliderLabel.setJustificationType(juce::Justification::centredBottom);
}

void DubSirenComponent::buttonClicked(juce::Button* button) {
    if (button = &trigger)
        dubSiren->setTrigger(button->getToggleState());
}

void DubSirenComponent::sliderValueChanged(juce::Slider* slider) {
    dubSiren->setFrequency(frequencySlider.getValue());
    dubSiren->setLfoFrequency(lfoFrequencySlider.getValue());
    dubSiren->setVolume(volumeSlider.getValue());
    val.setText(std::to_string(slider->getValue()), juce::NotificationType{});
}

void DubSirenComponent::sliderDragStarted(juce::Slider* slider) {
    val.attachToComponent(slider, juce::Justification::centredLeft);
}

void DubSirenComponent::sliderDragEnded(juce::Slider* slider) {
    val.setText("", juce::NotificationType{});
}

void DubSirenComponent::handleMidi(int control, int value) {
    const juce::MessageManagerLock mmLock;
    switch (control) {
    case 37:
        trigger.triggerClick();
        break;
    case 20:
        frequencySlider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.f, 1000.0f));
        break;
    case 21:
        lfoFrequencySlider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.f, 2.f));
        break;
    case 22:
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
    frequencySlider.setBounds(getWidth() * 0.1,
        getHeight() * 0.25,
        getWidth() * 0.3,
        getHeight() * 0.4);

    
    lfoFrequencySlider.setBounds(getWidth() * 0.6,
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
