/*
  ==============================================================================

    MeterComponent.cpp
    Created: 18 Feb 2024 2:16:33pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MeterComponent.h"

//==============================================================================
MeterComponent::MeterComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

MeterComponent::~MeterComponent()
{
}

void MeterComponent::setLevel(const float val) {
        level = val;
}

void MeterComponent::paint (juce::Graphics& g)
{
    juce::Rectangle<float> bounds = getLocalBounds().toFloat();

    g.setColour(juce::Colours::white);
    g.fillRect(bounds);

    g.setColour(juce::Colours::orange);

    const auto scale = juce::jmap(level, -60.f, +6.f, 0.f, (float)getHeight());
    g.fillRect(bounds.removeFromBottom(scale));
}

void MeterComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
