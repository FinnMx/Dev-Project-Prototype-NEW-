/*
  ==============================================================================

    PopoutWindow.cpp
    Created: 6 Mar 2024 12:46:47pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PopoutWindow.h"

//==============================================================================
PopoutWindow::PopoutWindow(const juce::String& name, juce::Component* component, int x, int y) : DocumentWindow(name, juce::Colours::aqua, 4), component(component)
{
    setUsingNativeTitleBar(true);
    setContentComponent(component);
    setResizable(false, false);
    centreWithSize(x * 0.4, y * 0.4);
    component->centreWithSize(getWidth(), getHeight());
}

PopoutWindow::~PopoutWindow()
{
}

void PopoutWindow::closeButtonPressed() {
    setVisible(false);
}

void PopoutWindow::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PopoutWindow", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PopoutWindow::resized()
{
}
