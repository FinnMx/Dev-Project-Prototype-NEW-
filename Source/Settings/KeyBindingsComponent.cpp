/*
  ==============================================================================

    KeyBindingsComponent.cpp
    Created: 4 Mar 2024 4:11:59pm
    Author:  New Owner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KeyBindingsComponent.h"

//==============================================================================
KeyBindingsComponent::KeyBindingsComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

KeyBindingsComponent::~KeyBindingsComponent()
{
}

void KeyBindingsComponent::paint (juce::Graphics& g)
{

    g.fillAll(getLookAndFeel().findColour(juce::PropertyComponent::backgroundColourId));

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("KeyBindingsComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void KeyBindingsComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
