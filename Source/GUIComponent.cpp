/*
  ==============================================================================

    GUIComponent.cpp
    Created: 17 Feb 2024 8:11:19pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GUIComponent.h"

//==============================================================================
GUIComponent::GUIComponent():
    inputPanel(),
    masterPanel(),
    thumbnailPanel(),
    tenBandPanel(),
    reverbPanel(),
    delayPanel(),
    killEQPanel()

{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

GUIComponent::~GUIComponent()
{
}

void GUIComponent::paint (juce::Graphics& g)
{
    juce::Colour panelBackground = getLookAndFeel().findColour(juce::Toolbar::backgroundColourId);

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component 

    g.setColour(panelBackground); g.drawRect(inputPanel); g.fillRect(inputPanel); g.setFont(juce::Font(20.0f)); g.setColour(juce::Colours::white); g.drawFittedText("Inputs", inputPanel, juce::Justification::centredTop, 10);
    g.setColour(panelBackground); g.drawRect(masterPanel); g.fillRect(masterPanel); g.setFont(juce::Font(20.0f)); g.setColour(juce::Colours::white); g.drawFittedText("Master", masterPanel, juce::Justification::centredTop, 10);
    g.setColour(panelBackground); g.drawRect(thumbnailPanel); g.fillRect(thumbnailPanel); g.setFont(juce::Font(20.0f)); g.setColour(juce::Colours::white); g.drawFittedText("Thumbnails", thumbnailPanel, juce::Justification::centredTop, 10);
    g.setColour(panelBackground); g.drawRect(tenBandPanel); g.fillRect(tenBandPanel); g.setFont(juce::Font(20.0f)); g.setColour(juce::Colours::white); g.drawFittedText("10 Band EQ", tenBandPanel, juce::Justification::centredTop, 10);
    g.setColour(panelBackground); g.drawRect(reverbPanel); g.fillRect(reverbPanel); g.setFont(juce::Font(20.0f)); g.setColour(juce::Colours::white); g.drawFittedText("Reverb", reverbPanel, juce::Justification::centredTop, 10);
    g.setColour(panelBackground); g.drawRect(delayPanel); g.fillRect(delayPanel); g.setFont(juce::Font(20.0f)); g.setColour(juce::Colours::white); g.drawFittedText("Delay", delayPanel, juce::Justification::centredTop, 10);
    g.setColour(panelBackground); g.drawRect(killEQPanel); g.fillRect(killEQPanel); g.setFont(juce::Font(20.0f)); g.setColour(juce::Colours::white); g.drawFittedText("Kill EQ", killEQPanel, juce::Justification::centredTop, 10);
}

void GUIComponent::resized()
{
    inputPanel.setBounds(
        getX(),
        getHeight() * 0.035,
        getWidth() * 0.25,
        getHeight() * 0.35
    );

    masterPanel.setBounds(
        getWidth() * 0.25,
        getHeight() * 0.035,
        getWidth() * 0.15,
        getHeight() * 0.35
    );

    thumbnailPanel.setBounds(
        getWidth() * 0.4,
        getHeight() * 0.035,
        getWidth() * 0.375,
        getHeight() * 0.35
    );

    tenBandPanel.setBounds(
        getWidth() * 0.775,
        getHeight() * 0.035,
        getWidth() * 0.225,
        getHeight() * 0.35
    );

    //-----------------------------------------------
    //Middle Rack's

    reverbPanel.setBounds(
        getX(),
        getHeight() * 0.39,
        getWidth(),
        getHeight() * 0.125
    );

    delayPanel.setBounds(
        getX(),
        getHeight() * 0.52,
        getWidth(),
        getHeight() * 0.125
    );

    //-----------------------------------------------
    //Bottom Rack

    killEQPanel.setBounds(
        getX(),
        getHeight() * 0.65,
        getWidth(),
        getHeight() * 0.34
    );
}
