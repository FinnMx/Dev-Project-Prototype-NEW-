/*
  ==============================================================================

    TenBandComponent.cpp
    Created: 15 Feb 2024 9:38:04pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TenBandComponent.h"

//==============================================================================
TenBandComponent::TenBandComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

TenBandComponent::~TenBandComponent()
{
}

void TenBandComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (getLookAndFeel().findColour(juce::TextEditor::outlineColourId).contrasting(0.15f));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(30.0f);
    g.drawText("10 Band EQ", getLocalBounds(),
        juce::Justification::centredTop, true);
}

void TenBandComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
