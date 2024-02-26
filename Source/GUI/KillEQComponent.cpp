/*
  ==============================================================================

    KillEQComponent.cpp
    Created: 15 Feb 2024 9:33:07pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KillEQComponent.h"

//==============================================================================
KillEQComponent::KillEQComponent()
{
    addAndMakeVisible(demo);

    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

KillEQComponent::~KillEQComponent()
{
}

void KillEQComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {

}

void KillEQComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    demo.getNextAudioBlock(bufferToFill);
}

void KillEQComponent::releaseResources() {

}

void KillEQComponent::paint (juce::Graphics& g)
{
    demo.repaint();
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (getLookAndFeel().findColour(juce::TextEditor::outlineColourId).contrasting(0.15f));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(30.0f);
    g.drawText("Kill EQ", getLocalBounds(),
        juce::Justification::centredTop, true);
}

void KillEQComponent::resized()
{
    demo.setBounds(getWidth() * 0.05,
        getHeight() * 0.25,
        getWidth() * 0.25,
        getHeight() * 0.5);

}
