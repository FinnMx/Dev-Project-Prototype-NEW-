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
    addAndMakeVisible(timeSlider);
    addAndMakeVisible(frequencyCutSlider);
    addAndMakeVisible(onOff);
    addAndMakeVisible(visualiser);

    gainSlider.addListener(this);
    timeSlider.addListener(this);
    frequencyCutSlider.addListener(this);
    onOff.addListener(this);

}

DelayComponent::~DelayComponent()
{
}

void DelayComponent::handleMidi(int control, int value) {
    const juce::MessageManagerLock mmLock;
    switch (control) {
    case 36:
        onOff.triggerClick();
        break;
    case 20:
        gainSlider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.f, 1.0f));
        break;
    case 21:
        timeSlider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.f, 1000.f));
        break;
    case 22:
        frequencyCutSlider.setValue(juce::jmap((float)value, (float)0, (float)127, 400.f, 18000.f));
        break;
    }
}

void DelayComponent::initSlider() {
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    gainSlider.setRange(0.f, +1.0f, 0.01f);
    gainSlider.setValue(0.f);
    gainSliderLabel.attachToComponent(&gainSlider, false);
    gainSliderLabel.setJustificationType(juce::Justification::centredBottom);

    frequencyCutSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    frequencyCutSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    frequencyCutSlider.setRange(1000.f, +19250.0f, 0.01f);
    frequencyCutSlider.setValue(19250.f);
    frequencyCutSliderLabel.attachToComponent(&frequencyCutSlider, false);
    frequencyCutSliderLabel.setJustificationType(juce::Justification::centredBottom);
    
    timeSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    timeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    timeSlider.setRange(0.f, +1000.f, 0.01f);
    timeSlider.setValue(0.f);
    timeSliderLabel.attachToComponent(&timeSlider, false);
    timeSliderLabel.setJustificationType(juce::Justification::centredBottom);
}

void DelayComponent::buttonClicked(juce::Button* button) {
    if (button = &onOff)
        circularBuffer->setDelayStatus(button->getToggleState());
   
}

void DelayComponent::sliderValueChanged(juce::Slider* slider) {
    circularBuffer->setDelayTime(timeSlider.getValue());
    circularBuffer->setDelayFeedback(gainSlider.getValue());
    circularBuffer->setDelayCutoffFrequency(frequencyCutSlider.getValue());
    val.setText(std::to_string(slider->getValue()), juce::NotificationType{});
}

void  DelayComponent::sliderDragStarted(juce::Slider* slider) {
    val.attachToComponent(slider, juce::Justification::centredLeft);
}

void  DelayComponent::sliderDragEnded(juce::Slider* slider) {
    val.setText("", juce::NotificationType{});
}


void DelayComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {

}

void DelayComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    visualiser.getCoefficients(circularBuffer->getCoefficients());
    visualiser.getNextAudioBlock(bufferToFill);
}

void DelayComponent::releaseResources() {

}

void DelayComponent::setFocus(bool newFocus) {
    const juce::MessageManagerLock mmLock;
    isFocused = newFocus;
    repaint();
}

void DelayComponent::paint (juce::Graphics& g)
{

    visualiser.repaint();
    if (isFocused)
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).withAlpha(.1f));   // clear the background
    if (!isFocused)
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

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

    timeSlider.setBounds(
        getWidth() * 0.3,
        getHeight() * 0.2,
        getWidth() * 0.2,
        getHeight() * 0.8
    );

    frequencyCutSlider.setBounds(getWidth() * 0.5,
        getHeight() * 0.2,
        getWidth() * 0.2,
        getHeight() * 0.8
    );

    onOff.setBounds(getWidth() * 0.7,
        getHeight() * 0.2,
        getWidth() * 0.5,
        getHeight() * 0.5);

    visualiser.setBounds(getWidth() * 0.75,
        getHeight() * 0.2,
        getWidth() * 0.2,
        getHeight() * 0.5);
}
