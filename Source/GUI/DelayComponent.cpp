/*
  ==============================================================================

    DelayComponent.cpp
    Created: 15 Feb 2024 9:33:24pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DelayComponent.h"

//==============================================================================
DelayComponent::DelayComponent(CircularBuffer* circularBuffer) : circularBuffer(circularBuffer)
{
    initSlider();

    addAndMakeVisible(gainSlider);
}

DelayComponent::~DelayComponent()
{
}

void  DelayComponent::initSlider() {
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    gainSlider.setRange(0.f, +3.0f, 0.01f);
    gainSlider.setValue(0.f);
    gainSliderLabel.attachToComponent(&gainSlider, false);
    gainSliderLabel.setJustificationType(juce::Justification::centredBottom);
}

void  DelayComponent::sliderValueChanged(juce::Slider* slider) {
    circularBuffer->setDelayGain(gainSlider.getValue());
    val.setText(std::to_string(slider->getValue()), juce::NotificationType{});
}

void  DelayComponent::sliderDragStarted(juce::Slider* slider) {
    val.attachToComponent(slider, juce::Justification::centredLeft);
}

void  DelayComponent::sliderDragEnded(juce::Slider* slider) {
    val.setText("", juce::NotificationType{});
}

void DelayComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (getLookAndFeel().findColour(juce::TextEditor::outlineColourId).contrasting(0.15f));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(30.0f);
    g.drawText("Delay", getLocalBounds(),
        juce::Justification::centredTop, true);
}



void DelayComponent::resized()
{
    gainSlider.setBounds(
        getWidth() * 0.1,
        getHeight() * 0.2,
        getWidth() * 0.2,
        getHeight() * 0.8
    );
}
