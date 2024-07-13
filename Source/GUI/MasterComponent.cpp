/*
  ==============================================================================

    MasterComponent.cpp
    Created: 15 Feb 2024 9:34:35pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MasterComponent.h"

//==============================================================================
MasterComponent::MasterComponent(float* rmsMasterLeft, float* rmsMasterRight, juce::MixerAudioSource* mixerSource, ExternalInput* externalInput) : 
    masterLeft(rmsMasterLeft), masterRight(rmsMasterRight), mixerSource(mixerSource), externalInput(externalInput)
{
    startTimerHz(30);
    addAndMakeVisible(rmsLeft);
    addAndMakeVisible(rmsRight);
    addAndMakeVisible(inputToggler);
    addAndMakeVisible(VolumeLabel);

    inputToggler.addListener(this);

}

MasterComponent::~MasterComponent()
{
}

void MasterComponent::buttonClicked(juce::Button* button) {
    externalInput->enableVolume(button->getToggleState());
}

void MasterComponent::toggleExternalInput() {
}

void MasterComponent::timerCallback() {
    rmsLeft.setLevel(*masterLeft);
    rmsRight.setLevel(*masterRight);

    rmsLeft.repaint();
    rmsRight.repaint();

    VolumeLabel.setText(juce::String(((int)*masterLeft + (int)*masterRight) / 2) + "Db", juce::NotificationType{});
}

void MasterComponent::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (getLookAndFeel().findColour(juce::TextEditor::outlineColourId).contrasting(0.15f));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(30.0f);
    g.drawText("Master", getLocalBounds(),
        juce::Justification::centredTop, true);
}

void MasterComponent::resized()
{
    rmsLeft.setBounds(
        getWidth() * 0.2,
        getHeight() * 0.175,
        getWidth() * 0.1,
        getHeight() * 0.6
    );

    rmsRight.setBounds(
        getWidth() * 0.325,
        getHeight() * 0.175,
        getWidth() * 0.1,
        getHeight() * 0.6
    );

    inputToggler.setBounds(
        getWidth() * 0.2,
        getHeight() * 0.8,
        getWidth() * 0.1,
        getHeight() * 0.1
    );

    VolumeLabel.setBounds(getWidth() * 0.275,
        getHeight() * 0.7,
        getWidth() * 0.1,
        getHeight() * 0.1);
    VolumeLabel.setSize(100, 100);

}
