/*
  ==============================================================================

    InputComponent.cpp
    Created: 15 Feb 2024 9:33:49pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "InputComponent.h"

//==============================================================================
InputComponent::InputComponent(std::string id, TrackAudioPlayer* audioPlayer, juce::AudioFormatManager& formatManager) :
    trackLoad("^"),
    trackPlay(">"),
    trackPause("| |"),
    trackStop("[]"),
    player(audioPlayer),
    trackName(id)
{
    addAndMakeVisible(trackLoad);
    addAndMakeVisible(trackPlay);
    addAndMakeVisible(trackPause);
    addAndMakeVisible(trackStop);

    trackLoad.addListener(this);
    trackPlay.addListener(this);
    trackPause.addListener(this);
    trackStop.addListener(this);
}

InputComponent::~InputComponent()
{
}

void InputComponent::buttonClicked(juce::Button* button) {
    if (button == &trackLoad) {
        juce::FileChooser chooser("Select a file to load for " + trackName,juce::File::getSpecialLocation(juce::File::userMusicDirectory), "*.wav; *.mp3; *.flac;");
        if (chooser.browseForFileToOpen()) {
            player->loadFile(chooser.getResult());
        }
    }
    if (button == &trackPlay) {
        player->play();
    }
    if (button == &trackStop) {
        player->stop();
    }
}

void InputComponent::sliderValueChanged(juce::Slider* slider) {

}

void InputComponent::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (getLookAndFeel().findColour(juce::TextEditor::outlineColourId).contrasting(0.15f));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(30.0f);
    g.drawText(trackName, getLocalBounds(),
        juce::Justification::centredTop, true);
}

void InputComponent::resized()
{
    // ----------------------------------
    // TRACK A BOUNDS

    trackLoad.setBounds(
        getWidth() * 0.04,
        getHeight() * 0.175,
        getWidth() * 0.125,
        getHeight() * 0.1
    );

    trackPlay.setBounds(
        getWidth() * 0.04,
        getHeight() * 0.3,
        getWidth() * 0.125,
        getHeight() * 0.1
    );

    trackPause.setBounds(
        getWidth() * 0.04,
        getHeight() * 0.425,
        getWidth() * 0.125,
        getHeight() * 0.1
    );

    trackStop.setBounds(
        getWidth() * 0.04,
        getHeight() * 0.55,
        getWidth() * 0.125,
        getHeight() * 0.1
    );

}
