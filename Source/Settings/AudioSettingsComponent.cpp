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
AudioSettingsComponent::AudioSettingsComponent() : apply("Apply"),
                                                   close("Close")
{

    setCentrePosition(400, 400);
    setVisible(true);

    addAndMakeVisible(close);
    addAndMakeVisible(apply);
    apply.addListener(this);
    close.addListener(this);
}

AudioSettingsComponent::~AudioSettingsComponent()
{
}

void AudioSettingsComponent::buttonClicked(juce::Button* button) {
    if (button == &apply) {

    }
    else if (button == &close) {
        delete this;
    }
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

    close.setBounds(getWidth() * 0.8,
        getHeight() * 0.8,
        getWidth() * 0.2,
        getHeight() * 0.2);

}
