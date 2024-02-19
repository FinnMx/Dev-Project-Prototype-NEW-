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
TrackThumbnailComponent::TrackThumbnailComponent(std::string id, juce::AudioThumbnailCache& cache, juce::AudioFormatManager& formatManager, TrackAudioPlayer* audioPlayer) :
    thumbnail(1000,formatManager,cache),
    isLoaded(false),
    id(id),
    player(audioPlayer)
{
    startTimerHz(1);
    thumbnail.addChangeListener(this);
    timeSlider.addListener(this);

    initTimeSliderSlider();
    addAndMakeVisible(timeSlider);

}

TrackThumbnailComponent::~TrackThumbnailComponent()
{
}

void TrackThumbnailComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    repaint();
}

void TrackThumbnailComponent::sliderValueChanged(juce::Slider* slider) {
    
}

void TrackThumbnailComponent::sliderDragStarted(juce::Slider* slider) {
    stopTimer();
}

void TrackThumbnailComponent::sliderDragEnded(juce::Slider* slider) {
    player->setTrackTime(timeSlider.getValue());
    startTimerHz(1);
}

void TrackThumbnailComponent::timerCallback() {
    timeSlider.setValue(player->getCurrentTrackTime());
}

void TrackThumbnailComponent::initTimeSliderSlider() {
    timeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    timeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, NULL, NULL);
}

void TrackThumbnailComponent::loadFile(juce::File file) {
    thumbnail.clear();

    isLoaded = thumbnail.setSource(new juce::FileInputSource(file));

    if (isLoaded) {
        repaint();
        timeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 25);
        timeSlider.setRange(00.00f, player->getTrackLength(),1.f);
        timeSlider.setValue(0.f);
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
        thumbnail.drawChannel(g, backDrop, 0, thumbnail.getTotalLength(), 0, 1.0f);
    }
    else {
        g.setFont(16.f);
        g.drawText("No File Is Loaded", getLocalBounds(), juce::Justification::centred,true);
    }
}

void TrackThumbnailComponent::resized()
{
    timeSlider.setBounds(getWidth() * 0.05,
        getHeight() * 0.6,
        getWidth() * 0.9,
        getHeight() * 0.3);

    backDrop.setBounds((int)std::round(getWidth() * 0.05),
        (int)std::round(getHeight() * 0.25),
        getWidth() * 0.9,
        getHeight() * 0.3);
}
