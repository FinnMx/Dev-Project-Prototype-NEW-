/*
  ==============================================================================

    AudioSettingsComponent.cpp
    Created: 4 Mar 2024 4:11:48pm
    Author:  New Owner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioSettingsComponent.h"

//==============================================================================
AudioSettingsComponent::AudioSettingsComponent() : apply("Apply")
{

    setCentrePosition(400, 400);
    setVisible(true);

    addAndMakeVisible(apply);
    apply.addListener(this);
}

AudioSettingsComponent::~AudioSettingsComponent()
{
}

void AudioSettingsComponent::buttonClicked(juce::Button* button) {
    if (button == &apply) {

    }
}

void AudioSettingsComponent::setAudioSettings(juce::AudioDeviceSelectorComponent* newAudioSettings) {
    audioSettings = newAudioSettings;
    addAndMakeVisible(audioSettings);
    resized();
}

void AudioSettingsComponent::paint (juce::Graphics& g)
{

    g.fillAll(getLookAndFeel().findColour(juce::PropertyComponent::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("AudioSettingsComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void AudioSettingsComponent::resized()
{
    audioSettings->setBounds(getWidth() * 0.5,
        getY(),
        getWidth() * 0.4,
        getHeight() * 0.4);
}
