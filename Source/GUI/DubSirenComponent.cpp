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
DubSirenComponent::DubSirenComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

DubSirenComponent::~DubSirenComponent()
{
}

void DubSirenComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("DubSirenComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void DubSirenComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
