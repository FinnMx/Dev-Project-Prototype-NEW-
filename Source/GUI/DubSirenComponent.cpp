/*
  ==============================================================================

    DubSirenComponent.cpp
    Created: 28 Feb 2024 4:33:17pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DubSirenComponent.h"

//==============================================================================
DubSirenComponent::DubSirenComponent(DubSiren* dubSiren) : dubSiren(dubSiren)
{
    addAndMakeVisible(trigger);
    trigger.addListener(this);
}

DubSirenComponent::~DubSirenComponent()
{
}

void DubSirenComponent::buttonClicked(juce::Button* button) {
    if (button = &trigger)
        dubSiren->setTrigger(button->getToggleState());
}

void DubSirenComponent::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(getLookAndFeel().findColour(juce::TextEditor::outlineColourId).contrasting(0.15f));
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(30.0f);
    g.drawText("Dub Siren", getLocalBounds(),
        juce::Justification::centredTop, true);
}

void DubSirenComponent::resized()
{
    trigger.setBounds(getWidth() * 0.7,
        getHeight() * 0.2,
        getWidth() * 0.5,
        getHeight() * 0.5);
}
