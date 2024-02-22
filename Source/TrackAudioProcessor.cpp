/*
  ==============================================================================

    TrackAudioProcessor.cpp
    Created: 22 Feb 2024 5:21:45pm
    Author:  New Owner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TrackAudioProcessor.h"

//==============================================================================
TrackAudioProcessor::TrackAudioProcessor()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

TrackAudioProcessor::~TrackAudioProcessor()
{
}

const juce::String TrackAudioProcessor::getName() const {
    return "audioProcessor";
}

void TrackAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    auto delayBufferSize = sampleRate * 2.0;
    delayBuffer.setSize(getTotalNumOutputChannels(), (int)delayBufferSize);
}

void TrackAudioProcessor::releaseResources() {

}

void TrackAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    auto bufferSize = buffer.getNumSamples();
    auto delayBufferSize = delayBuffer.getNumSamples();

    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer(channel);

        if (delayBufferSize > bufferSize + writePosition) {
            delayBuffer.copyFromWithRamp(channel, writePosition, channelData, bufferSize, 0.1f, 0.1f);
        }
        else {
            // Figure out how much space is left at the end of the buffer
            auto numSamplesRemaining = delayBufferSize - writePosition;

            delayBuffer.copyFromWithRamp(channel, writePosition, channelData, numSamplesRemaining, 0.1f, 0.1f);

            auto numSamplesAtStart = bufferSize - numSamplesRemaining;
            // Copy remaining samples to the start (start of the "loop"/"circle")
            delayBuffer.copyFromWithRamp(channel, 0, channelData, numSamplesAtStart, 0.1f, 0.1f);
        }
    }

    DBG(delayBufferSize);
    DBG(bufferSize);
    DBG(writePosition);

    writePosition += bufferSize;
    writePosition %= delayBufferSize;

}

double TrackAudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

bool TrackAudioProcessor::acceptsMidi() const {
    #if JucePlugin_WantsMidiInput
        return true;
    #else
        return false;
    #endif
}

bool TrackAudioProcessor::producesMidi() const {
    #if JucePlugin_ProducesMidiOutput
        return true;
    #else
        return false;
    #endif
}

juce::AudioProcessorEditor* TrackAudioProcessor::createEditor() {
    return nullptr;
}

bool TrackAudioProcessor::hasEditor() const {
    return false;
}

int TrackAudioProcessor::getNumPrograms() {
    return 1;
}

int TrackAudioProcessor::getCurrentProgram() {
    return 0;
}

void TrackAudioProcessor::setCurrentProgram(int index) {

}

const juce::String TrackAudioProcessor::getProgramName(int index) {
    return {};
}

void TrackAudioProcessor::changeProgramName(int index, const juce::String& newName) {

}

void TrackAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {

}

void TrackAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {

}