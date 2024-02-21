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
    addAndMakeVisible(dryLevelSlider);

    roomSizeSlider.addListener(this);
    dampingSlider.addListener(this);
    wetLevelSlider.addListener(this);
    dryLevelSlider.addListener(this);
}

void ReverbComponent::initSlider() {
    roomSizeSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    roomSizeSlider.setTextBoxStyle(juce::Slider::NoTextBox,true, NULL, NULL);
    roomSizeSlider.setRange(0.f, +1.0f, 0.01f);
    roomSizeSlider.setValue(0.f);

    dampingSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    dampingSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    dampingSlider.setRange(0.f, +1.f, 0.01f);
    dampingSlider.setValue(0.f);

    wetLevelSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    wetLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    wetLevelSlider.setRange(0.f, +1.f, 0.01f);
    wetLevelSlider.setValue(0.f);

    dryLevelSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    dryLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
    dryLevelSlider.setRange(0.f, +1.f, 0.01f);
    dryLevelSlider.setValue(0.f);
}

ReverbComponent::~ReverbComponent()
{
}

void ReverbComponent::sliderValueChanged(juce::Slider* slider) {
    source1->setReverbParams(roomSizeSlider.getValue(), dampingSlider.getValue(), wetLevelSlider.getValue(), dryLevelSlider.getValue());
    source2->setReverbParams(roomSizeSlider.getValue(), dampingSlider.getValue(), wetLevelSlider.getValue(), dryLevelSlider.getValue());
}

void ReverbComponent::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

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
        getHeight() * 0.25,
        getWidth() * 0.2,
        getHeight() * 0.5
    );

    dampingSlider.setBounds(
        getWidth() * 0.3,
        getHeight() * 0.25,
        getWidth() * 0.2,
        getHeight() * 0.5
    );

    wetLevelSlider.setBounds(
        getWidth() * 0.5,
        getHeight() * 0.25,
        getWidth() * 0.2,
        getHeight() * 0.5
    );

    dryLevelSlider.setBounds(
        getWidth() * 0.7,
        getHeight() * 0.25,
        getWidth() * 0.2,
        getHeight() * 0.5
    );


}
