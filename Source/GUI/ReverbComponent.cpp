/*
  ==============================================================================

    ReverbComponent.cpp
    Created: 15 Feb 2024 9:33:35pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ReverbComponent.h"

//==============================================================================
ReverbComponent::ReverbComponent(TrackAudioPlayer* track1, TrackAudioPlayer* track2) : source1(track1), source2(track2)
{
    initSlider();

    addAndMakeVisible(roomSizeSlider);
    addAndMakeVisible(dampingSlider);
    addAndMakeVisible(wetLevelSlider);

    roomSizeSlider.addListener(this);
    dampingSlider.addListener(this);
    wetLevelSlider.addListener(this);
}

void ReverbComponent::initSlider() {
    roomSizeSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    roomSizeSlider.setTextBoxStyle(juce::Slider::NoTextBox,true, NULL, NULL);
    roomSizeSlider.setRange(0.f, +1.0f, 0.01f);
    roomSizeSlider.setValue(0.f);
    roomSizeLabel.attachToComponent(&roomSizeSlider, false);
    roomSizeLabel.setJustificationType(juce::Justification::centredBottom);
    roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 25);

    dampingSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    dampingSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    dampingSlider.setRange(0.f, +1.f, 0.01f);
    dampingSlider.setValue(0.f);
    dampingLabel.attachToComponent(&dampingSlider, false);
    dampingLabel.setJustificationType(juce::Justification::centredBottom);
    dampingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 25);

    wetLevelSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    wetLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    wetLevelSlider.setRange(0.f, +1.f, 0.01f);
    wetLevelSlider.setValue(0.f);
    WetLevelLabel.attachToComponent(&wetLevelSlider, false);
    WetLevelLabel.setJustificationType(juce::Justification::centredBottom);
    wetLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 25);
}

ReverbComponent::~ReverbComponent()
{
}

void ReverbComponent::handleMidi(int control, int value) {
    const juce::MessageManagerLock mmLock;
    switch (control) {
    case 20:
        roomSizeSlider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.f, 1.0f));
        break;
    case 21:
        dampingSlider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.f, 1.f));
        break;
    case 22:
        wetLevelSlider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.f, 1.f));
        break;
    }
}

void ReverbComponent::sliderValueChanged(juce::Slider* slider) {
    source1->setReverbParams(roomSizeSlider.getValue(), dampingSlider.getValue(), wetLevelSlider.getValue());
    source2->setReverbParams(roomSizeSlider.getValue(), dampingSlider.getValue(), wetLevelSlider.getValue());
    //val.setText(std::to_string(slider->getValue()), juce::NotificationType{});
}

void ReverbComponent::sliderDragStarted(juce::Slider* slider) {
    //JANK
    //val.attachToComponent(slider, juce::Justification::centredLeft);
}
void ReverbComponent::sliderDragEnded(juce::Slider* slider) {
    //JANK
    //val.setText("", juce::NotificationType{});
}

void ReverbComponent::setFocus(bool newFocus) {
    const juce::MessageManagerLock mmLock;
    isFocused = newFocus;
    repaint();
}

void ReverbComponent::paint (juce::Graphics& g)
{
    if (isFocused)
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).withAlpha(.1f));   // clear the background
    if (!isFocused)
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour (getLookAndFeel().findColour(juce::TextEditor::outlineColourId).contrasting(0.15f));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(30.0f);
    g.drawText("Reverb", getLocalBounds(),
        juce::Justification::centredTop, true);

}

void ReverbComponent::resized()
{
    roomSizeSlider.setBounds(
        getWidth() * 0.1,
        getHeight() * 0.2,
        getWidth() * 0.2,
        getHeight() * 0.8
    );

    dampingSlider.setBounds(
        getWidth() * 0.3,
        getHeight() * 0.2,
        getWidth() * 0.2,
        getHeight() * 0.8
    );

    wetLevelSlider.setBounds(
        getWidth() * 0.5,
        getHeight() * 0.2,
        getWidth() * 0.2,
        getHeight() * 0.8
    );

}
