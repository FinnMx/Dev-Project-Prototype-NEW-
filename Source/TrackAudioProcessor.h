/*
  ==============================================================================

    TrackAudioProcessor.h
    Created: 22 Feb 2024 5:21:45pm
    Author:  New Owner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TrackAudioProcessor  : public juce::AudioProcessor
{
public:
    TrackAudioProcessor();
    ~TrackAudioProcessor() override;

    const juce::String getName() const override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    double getTailLengthSeconds() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
 

private:
    void fillBuffer(int channel, int bufferSize, int delayBufferSize, float* channelData);

    juce::AudioBuffer<float> delayBuffer;
    int writePosition{0};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackAudioProcessor)
};
