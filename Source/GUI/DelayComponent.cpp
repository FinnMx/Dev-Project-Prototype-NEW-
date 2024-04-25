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
    addAndMakeVisible(lowFrequencyCutSlider);
    addAndMakeVisible(highFrequencyCutSlider);
    addAndMakeVisible(onOff);
    addAndMakeVisible(sirenSend);
    addAndMakeVisible(trackSend);
    addAndMakeVisible(onOff);
    addAndMakeVisible(visualiser);

    gainSlider.addListener(this);
    timeSlider.addListener(this);
    lowFrequencyCutSlider.addListener(this);
    highFrequencyCutSlider.addListener(this);
    onOff.addListener(this);
    sirenSend.addListener(this);
    trackSend.addListener(this);

}

DelayComponent::~DelayComponent()
{
}

void DelayComponent::handleMidi(int action, int value) {
    const juce::MessageManagerLock mmLock;
    switch (action) {
    case 0:
        gainSlider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.f, 1.0f));
        break;
    case 1:
        timeSlider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.f, 1000.f));
        break;
    case 2:
        highFrequencyCutSlider.setValue(juce::jmap((float)value, (float)0, (float)127, 250.f, 19250.0f));
        break;
    case 3:
        lowFrequencyCutSlider.setValue(juce::jmap((float)value, (float)0, (float)127, 1000.f, 19250.0f));
        break;
    case 4:
        onOff.triggerClick();
        break;
    }
}

void DelayComponent::initSlider() {
    //FEEDBACK!!!! NOT GAIN!!!!!
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    gainSlider.setRange(0.f, +1.0f, 0.01f);
    gainSlider.setValue(0.f);
    gainSliderLabel.attachToComponent(&gainSlider, false);
    gainSliderLabel.setJustificationType(juce::Justification::centredBottom);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 25);

    lowFrequencyCutSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    lowFrequencyCutSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    lowFrequencyCutSlider.setRange(1000.f, +19250.0f, 0.01f);
    lowFrequencyCutSlider.setValue(19250.f);
    lowFrequencyCutSliderLabel.attachToComponent(&lowFrequencyCutSlider, false);
    lowFrequencyCutSliderLabel.setJustificationType(juce::Justification::centredBottom);
    lowFrequencyCutSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 25);

    highFrequencyCutSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    highFrequencyCutSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    highFrequencyCutSlider.setRange(250.f, +19250.0f, 0.01f);
    highFrequencyCutSlider.setValue(250.f);
    highFrequencyCutSliderLabel.attachToComponent(&highFrequencyCutSlider, false);
    highFrequencyCutSliderLabel.setJustificationType(juce::Justification::centredBottom);
    highFrequencyCutSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 25);
    
    timeSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    timeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    timeSlider.setRange(0.f, +1000.f);
    timeSlider.setValue(0.f);
    timeSliderLabel.attachToComponent(&timeSlider, false);
    timeSliderLabel.setJustificationType(juce::Justification::centredBottom);
    timeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 25);
}

void DelayComponent::buttonClicked(juce::Button* button) {
    if (button == &onOff)
        circularBuffer->setDelayStatus(button->getToggleState());
    if (button == &trackSend)
        circularBuffer->updateTrackSendStatus(button->getToggleState());
    if (button == &sirenSend)
        circularBuffer->updateSirenSendStatus(button->getToggleState());
   
}

void DelayComponent::sliderValueChanged(juce::Slider* slider) {
    circularBuffer->setDelayTime(timeSlider.getValue());
    circularBuffer->setDelayFeedback(gainSlider.getValue());
    circularBuffer->setDelayLowCutoffFrequency(lowFrequencyCutSlider.getValue());
    circularBuffer->setDelayHighCutoffFrequency(highFrequencyCutSlider.getValue());
    //val.setText(std::to_string(slider->getValue()), juce::NotificationType{});
}

void  DelayComponent::sliderDragStarted(juce::Slider* slider) {
    //val.attachToComponent(slider, juce::Justification::centredLeft);
}

void  DelayComponent::sliderDragEnded(juce::Slider* slider) {
    //val.setText("", juce::NotificationType{});
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

    highFrequencyCutSlider.setBounds(getWidth() * 0.5,
        getHeight() * 0.3,
        getWidth() * 0.1,
        getHeight() * 0.5
    );

    lowFrequencyCutSlider.setBounds(getWidth() * 0.6,
        getHeight() * 0.3,
        getWidth() * 0.1,
        getHeight() * 0.5
    );

    onOff.setBounds(getWidth() * 0.7,
        getHeight() * 0.2,
        getWidth() * 0.5,
        getHeight() * 0.5);

    sirenSend.setBounds(getWidth() * 0.05,
        getHeight() * 0.05,
        getWidth() * 0.1,
        getHeight() * 0.1);

    trackSend.setBounds(getWidth() * 0.05,
        getHeight() * 0.5,
        getWidth() * 0.1,
        getHeight() * 0.1);

    visualiser.setBounds(getWidth() * 0.75,
        getHeight() * 0.2,
        getWidth() * 0.2,
        getHeight() * 0.5);
}
