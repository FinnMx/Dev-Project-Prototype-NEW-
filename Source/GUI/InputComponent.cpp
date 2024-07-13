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
InputComponent::InputComponent(std::string id, TrackAudioPlayer* audioPlayer, juce::AudioFormatManager& formatManager, TrackThumbnailComponent* trackThumb) :
    trackLoad("^"),
    trackPlay(">"),
    trackPause("| |"),
    trackStop("[]"),
    player(audioPlayer),
    trackName(id),
    trackThumbnail(trackThumb)
{
    startTimerHz(30);

    addAndMakeVisible(trackLoad);
    addAndMakeVisible(trackPlay);
    addAndMakeVisible(trackPause);
    addAndMakeVisible(trackStop);

    addAndMakeVisible(MeterL);
    addAndMakeVisible(MeterR);
    addAndMakeVisible(VolumeLabel);

    initVolSlider();
    addAndMakeVisible(volSlider);

    trackLoad.addListener(this);
    trackPlay.addListener(this);
    trackPause.addListener(this);
    trackStop.addListener(this);

    volSlider.addListener(this);
}

InputComponent::~InputComponent()
{
}

void InputComponent::handleMidi(int action, int value) {
    const juce::MessageManagerLock mmLock;
    switch (action) {
    case 0:
        trackLoad.triggerClick();
        break;
    case 1:
        trackPlay.triggerClick();
        break;
    case 2:
        trackPause.triggerClick();
        break;
    case 3:
        trackStop.triggerClick();
        break;
    case 4:
        volSlider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.f, 1.0f));
        break;
    }
}

void InputComponent::initVolSlider() {
    volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 25);

    //SHOULD PROBABLY CHANGE THIS SO ITS IN A RANGE OF -60 TO +60DB!!!!!! SEE THE JUCE::AUDIOTRANSPORTPLAYER.SETGAIN FUNC!!!
    volSlider.setRange(0.f, +1.f, 0.01f);
    volSlider.setValue(1.f);
}

void InputComponent::buttonClicked(juce::Button* button) {
    if (button == &trackLoad) {
        juce::FileChooser chooser("Select a file to load for " + trackName,juce::File::getSpecialLocation(juce::File::userMusicDirectory), "*.wav; *.mp3; *.flac;");
        if (chooser.browseForFileToOpen()) {
            player->loadFile(chooser.getResult());
            trackThumbnail->loadFile(chooser.getResult());
        }
    }
    if (button == &trackPlay) {
        player->play();
    }
    if (button == &trackStop) {
        player->stop();
    }
    if (button == &trackPause) {
        player->pause();
    }
}

void InputComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &volSlider) {
        player->setGain(volSlider.getValue());
    }
}

void InputComponent::timerCallback() {

    float lRMS = player->getRMSValue(0);
    float rRMS = player->getRMSValue(1);
    int RMS = ((int)lRMS + (int)rRMS) / 2;

    MeterL.setLevel(lRMS);
    MeterR.setLevel(rRMS);

    MeterL.repaint();
    MeterR.repaint();

    VolumeLabel.setText(juce::String(RMS) + "Db", juce::NotificationType{});
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

    MeterL.setBounds(
        getWidth() * 0.2,
        getHeight() * 0.175,
        getWidth() * 0.1,
        getHeight() * 0.6
    );

    MeterR.setBounds(
        getWidth() * 0.325,
        getHeight() * 0.175,
        getWidth() * 0.1,
        getHeight() * 0.6
    );
   

    volSlider.setBounds(
        getWidth() * 0.6,
        getHeight() * 0.175,
        getWidth() * 0.2,
        getHeight() * 0.6
    );

    VolumeLabel.setBounds(getWidth() * 0.275,
        getHeight() * 0.7,
        getWidth() * 0.1,
        getHeight() * 0.1);
    VolumeLabel.setSize(100, 100);
}
