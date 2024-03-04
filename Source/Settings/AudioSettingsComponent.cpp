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
AudioSettingsComponent::AudioSettingsComponent()
{

}

AudioSettingsComponent::~AudioSettingsComponent()
{
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
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
