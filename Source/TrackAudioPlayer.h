/*
  ==============================================================================

    TrackAudioPlayer.h
    Created: 18 Feb 2024 1:14:55am
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TrackAudioPlayer  : public juce::AudioSource
{
public:
    TrackAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~TrackAudioPlayer() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadFile(juce::File _file);
    void play();
    void pause();
    void stop();

    double getTrackLength();
    double getCurrentTrackTime();
    void setTrackTime(double newTime);

    void setGain(float newGain);
    void setReverbParams(float roomSize, float damping, float wetLevel, float dryLevel);

    float getRMSValue(const int channel) const;

private:
    float rmsLvlLeft, rmsLvlRight;

    juce::AudioFormatManager& formatManager;

    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

    //Effects
    juce::ReverbAudioSource reverbSource{&resampleSource, false};
    juce::Reverb::Parameters parameters;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackAudioPlayer)
};
