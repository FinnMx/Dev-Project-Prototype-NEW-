/*
  ==============================================================================

    KillEQComponent.cpp
    Created: 15 Feb 2024 9:33:07pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KillEQComponent.h"

//==============================================================================
KillEQComponent::KillEQComponent(FrequencyCutoffs* freqCutoffs) : freqCutoffs(freqCutoffs)
{
    initSlider();

    addAndMakeVisible(visualiser);

    addAndMakeVisible(subBassFrequencySlider);
    addAndMakeVisible(bassFrequencySlider);
    addAndMakeVisible(midsFrequencySlider);
    addAndMakeVisible(highFrequencySlider);
    subBassFrequencySlider.addListener(this);
    bassFrequencySlider.addListener(this);
    midsFrequencySlider.addListener(this);
    highFrequencySlider.addListener(this);

    addAndMakeVisible(subBassOnOff);
    addAndMakeVisible(bassOnOff);
    addAndMakeVisible(midsOnOff);
    addAndMakeVisible(highOnOff);
    subBassOnOff.addListener(this);
    bassOnOff.addListener(this);
    midsOnOff.addListener(this);
    highOnOff.addListener(this);
}

KillEQComponent::~KillEQComponent()
{
}

void KillEQComponent::initSlider() {
    subBassFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    subBassFrequencySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    subBassFrequencySlider.setRange(20.f, +95.f, 1.f);
    subBassFrequencySlider.setValue(+95.f);
    subBassFrequencyLabel.attachToComponent(&subBassFrequencySlider, false);
    subBassFrequencyLabel.setJustificationType(juce::Justification::centredBottom);

    bassFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    bassFrequencySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    bassFrequencySlider.setRange(95.f, +250.f, 1.f);
    bassFrequencySlider.setValue(+250.f);
    bassFrequencyLabel.attachToComponent(&bassFrequencySlider, false);
    bassFrequencyLabel.setJustificationType(juce::Justification::centredBottom);

    midsFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    midsFrequencySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    midsFrequencySlider.setRange(250.f, +3500.f, 1.f);
    midsFrequencySlider.setValue(+3500.f);
    midsFrequencyLabel.attachToComponent(&midsFrequencySlider, false);
    midsFrequencyLabel.setJustificationType(juce::Justification::centredBottom);

    highFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    highFrequencySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    highFrequencySlider.setRange(3500.f, +20000.f, 1.f);
    highFrequencySlider.setValue(+5000.f);
    highFrequencyLabel.attachToComponent(&highFrequencySlider, false);
    highFrequencyLabel.setJustificationType(juce::Justification::centredBottom);
}

void KillEQComponent::handleMidi(int control) {
    const juce::MessageManagerLock mmLock;
    switch (control) {
    case 49:
        subBassOnOff.triggerClick();
        break;
    case 41:
        bassOnOff.triggerClick();
        break;
    case 42:
        midsOnOff.triggerClick();
        break;
    case 46:
        highOnOff.triggerClick();
        break;
    }
}

void KillEQComponent::sliderValueChanged(juce::Slider* slider) {
    freqCutoffs->setSubBassFilter(subBassFrequencySlider.getValue());
    freqCutoffs->setbassFilterr(bassFrequencySlider.getValue());
    freqCutoffs->setmidsFilter(midsFrequencySlider.getValue());
    freqCutoffs->sethighFilter(highFrequencySlider.getValue());
}

void KillEQComponent::buttonClicked(juce::Button* button) {
    if (button = &subBassOnOff)
        freqCutoffs->setSubBassStatus(button->getToggleState());
    if (button = &bassOnOff)
        freqCutoffs->setBassStatus(button->getToggleState());
    if (button = &midsOnOff)
        freqCutoffs->setMidsStatus(button->getToggleState());
    if (button = &highOnOff)
        freqCutoffs->setHighStatus(button->getToggleState());
}

void KillEQComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {

}

void KillEQComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    visualiser.getCoefficients(freqCutoffs->getCoefficients());
    visualiser.getNextAudioBlock(bufferToFill);
}

void KillEQComponent::releaseResources() {

}

void KillEQComponent::paint (juce::Graphics& g)
{
    visualiser.repaint();
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (getLookAndFeel().findColour(juce::TextEditor::outlineColourId).contrasting(0.15f));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(30.0f);
    g.drawText("Kill EQ", getLocalBounds(),
        juce::Justification::centredTop, true);
}

void KillEQComponent::resized()
{
    visualiser.setBounds(getWidth() * 0.05,
        getHeight() * 0.25,
        getWidth() * 0.25,
        getHeight() * 0.5);

    subBassFrequencySlider.setBounds(
        getWidth() * 0.3,
        getHeight() * 0.25,
        getWidth() * 0.2,
        getHeight() * 0.3
    );

    subBassOnOff.setBounds(
        getWidth() * 0.3,
        getHeight() * 0.5,
        getWidth() * 0.2,
        getHeight() * 0.3
    );

    bassFrequencySlider.setBounds(
        getWidth() * 0.45,
        getHeight() * 0.25,
        getWidth() * 0.2,
        getHeight() * 0.3
    );

    bassOnOff.setBounds(
        getWidth() * 0.45,
        getHeight() * 0.5,
        getWidth() * 0.2,
        getHeight() * 0.3
    );

    midsFrequencySlider.setBounds(
        getWidth() * 0.6,
        getHeight() * 0.25,
        getWidth() * 0.2,
        getHeight() * 0.3
    );

    midsOnOff.setBounds(
        getWidth() * 0.6,
        getHeight() * 0.5,
        getWidth() * 0.2,
        getHeight() * 0.3
    );

    highFrequencySlider.setBounds(
        getWidth() * 0.75,
        getHeight() * 0.25,
        getWidth() * 0.2,
        getHeight() * 0.3
    );

    highOnOff.setBounds(
        getWidth() * 0.75,
        getHeight() * 0.5,
        getWidth() * 0.2,
        getHeight() * 0.3
    );

}
