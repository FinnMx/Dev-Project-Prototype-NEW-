/*
  ==============================================================================

    FileBrowserComponent.cpp
    Created: 12 Feb 2024 1:50:50pm
    Author:  New Owner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FileBrowserComponent.h"

//==============================================================================
FileBrowserComponent::FileBrowserComponent() : fileBrowser(juce::FileBrowserComponent::openMode |
                                                           juce::FileBrowserComponent::canSelectFiles
                                                           ,juce::File("C:/"), NULL, NULL)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(&fileBrowser);
}

FileBrowserComponent::~FileBrowserComponent()
{
}

void FileBrowserComponent::paint (juce::Graphics& g)
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

}

void FileBrowserComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    fileBrowser.setBounds(getLocalBounds()); // Height if component

}
