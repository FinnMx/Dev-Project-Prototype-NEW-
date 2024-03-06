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
    setReverbParams(0.f,0.f,0.f);
}

TrackAudioPlayer::~TrackAudioPlayer()
{
}

void TrackAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate){
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void TrackAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbSource.releaseResources();
}

void TrackAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    reverbSource.getNextAudioBlock(bufferToFill);
    //resampleSource.getNextAudioBlock(bufferToFill);

    rmsLvlLeft = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(0, 0, bufferToFill.buffer->getNumSamples()));
    rmsLvlRight = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(1, 0, bufferToFill.buffer->getNumSamples()));
}

void TrackAudioPlayer::loadFile(juce::File _file) {
    double sampleRate = 44100.0;
    juce::AudioFormatReader* reader = formatManager.createReaderFor(_file);

    if (reader != nullptr) {
        // Prepares the file to play
        std::unique_ptr<juce::AudioFormatReaderSource> tempSource(new juce::AudioFormatReaderSource(reader, true));

        // Gets data from the audio file pointed to from tempSource. Currently specifying sample rates to stop incorrect playback
        transportSource.setSource(tempSource.get(), 0, nullptr, sampleRate);

        //prevents the audio from stopping when tempsource is destroyed, practically just passes the pointer to a differnet source
        readerSource.reset(tempSource.release());
    }
}

bool TrackAudioPlayer::getPlayingState() {
    return isPlaying;
}

void TrackAudioPlayer::play() {
    transportSource.start();
    isPlaying = true;
}

void TrackAudioPlayer::pause() {
    transportSource.stop();
}

void TrackAudioPlayer::stop() {
    transportSource.setPosition(0.0);
    transportSource.stop();
    isPlaying = false;

}

void TrackAudioPlayer::setGain(float newGain) {
    transportSource.setGain(newGain);
}

void TrackAudioPlayer::setReverbParams(float roomSize, float damping, float wetLevel){
    parameters.roomSize = roomSize;
    parameters.damping = damping;
    parameters.wetLevel = wetLevel;
    parameters.dryLevel = 1.f;
    reverbSource.setParameters(parameters);
}

void TrackAudioPlayer::setTrackTime(double newTime) {
    transportSource.setPosition(newTime);
}

double TrackAudioPlayer::getTrackLength() {
    return transportSource.getLengthInSeconds();
}

double TrackAudioPlayer::getCurrentTrackTime() {
    return transportSource.getCurrentPosition();
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


