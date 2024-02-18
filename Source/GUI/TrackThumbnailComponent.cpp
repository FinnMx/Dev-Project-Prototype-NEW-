/*
  ==============================================================================

    TrackThumbnailComponent.cpp
    Created: 15 Feb 2024 9:35:52pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TrackThumbnailComponent.h"

//==============================================================================
TrackThumbnailComponent::TrackThumbnailComponent(std::string id, juce::AudioThumbnailCache& cache, juce::AudioFormatManager& formatManager) :
    thumbnail(1000,formatManager,cache),
    isLoaded(false),
    id(id)
{
    thumbnail.addChangeListener(this);

}

TrackThumbnailComponent::~TrackThumbnailComponent()
{
}

void TrackThumbnailComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    repaint();
}

void TrackThumbnailComponent::loadFile(juce::File file) {
    thumbnail.clear();

    isLoaded = thumbnail.setSource(new juce::FileInputSource(file));

    if (isLoaded) {
        repaint();
    }
}

void TrackThumbnailComponent::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (getLookAndFeel().findColour(juce::TextEditor::outlineColourId).contrasting(0.15f));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(30.0f);
    g.drawText(id, getLocalBounds(),
        juce::Justification::centredTop, true);

    if (isLoaded) {
        thumbnail.drawChannel(g, getLocalBounds(), 0, thumbnail.getTotalLength(), 0, 1.0f);
    }
    else {
        g.setFont(16.f);
        g.drawText("No File Is Loaded", getLocalBounds(), juce::Justification::centred,true);
    }
}

void TrackThumbnailComponent::resized()
{

}
