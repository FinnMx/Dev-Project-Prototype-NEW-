/*
  ==============================================================================

    TrackAudioPlayer.cpp
    Created: 18 Feb 2024 1:14:55am
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TrackAudioPlayer.h"

//==============================================================================
TrackAudioPlayer::TrackAudioPlayer(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager)
{
    transportSource.setGain(0.5f);

}

TrackAudioPlayer::~TrackAudioPlayer()
{
}

void TrackAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate){
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void TrackAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void TrackAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    bufferToFill.clearActiveBufferRegion();

    transportSource.getNextAudioBlock(bufferToFill);

    rmsLvlLeft = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(0, 0, bufferToFill.buffer->getNumSamples()));
    rmsLvlRight = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(1, 0, bufferToFill.buffer->getNumSamples()));
}

void TrackAudioPlayer::loadFile(juce::File _file) {
    juce::AudioFormatReader* reader = formatManager.createReaderFor(_file);

    if (reader != nullptr) {
        // Prepares the file to play
        std::unique_ptr<juce::AudioFormatReaderSource> tempSource(new juce::AudioFormatReaderSource(reader, true));

        // Gets data from the audio file pointed to from tempSource.
        transportSource.setSource(tempSource.get());

        //prevents the audio from stopping when tempsource is destroyed, practically just passes the pointer to a differnet source
        readerSource.reset(tempSource.release());
    }
}

void TrackAudioPlayer::play() {
    transportSource.start();
}

void TrackAudioPlayer::pause() {
    transportSource.stop();
}

void TrackAudioPlayer::stop() {
    transportSource.stop();
    transportSource.setPosition(0.0);
}

void TrackAudioPlayer::setGain(float newGain) {
    transportSource.setGain(newGain);
}

float TrackAudioPlayer::getRMSValue(const int channel) const {
    jassert(channel == 0 || channel == 1);
        if (channel == 0) {
            return rmsLvlLeft;
        }
        if (channel == 1) {
            return rmsLvlRight;
        }
        else {
            return 0.f;
        }
    
}


