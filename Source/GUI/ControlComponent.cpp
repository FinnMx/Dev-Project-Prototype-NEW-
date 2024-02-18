/*
  ==============================================================================

    ControlComponent.cpp
    Created: 12 Feb 2024 3:38:14pm
    Author:  New Owner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ControlComponent.h"

//==============================================================================
ControlComponent::ControlComponent() :
    playButton("Play"),
    pauseButton("Pause"),
    stopButton("Stop")
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(stopButton);
}

ControlComponent::~ControlComponent()
{
}

void ControlComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
}

void ControlComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    playButton.setBounds(0, // X position reletive to main component
        0, // Y position reletive to main component
        getWidth() * 0.3, // Width of component
        getHeight());

    pauseButton.setBounds(getWidth() * 0.33333, // X position reletive to main component
        0, // Y position reletive to main component
        getWidth() * 0.3, // Width of component
        getHeight());

    stopButton.setBounds(getWidth() * 0.66666, // X position reletive to main component
        0, // Y position reletive to main component
        getWidth() * 0.3, // Width of component
        getHeight());
}
