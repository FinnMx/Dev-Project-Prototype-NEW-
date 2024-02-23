/*
  ==============================================================================

    CircularBuffer.cpp
    Created: 22 Feb 2024 11:01:45pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CircularBuffer.h"

//==============================================================================
CircularBuffer::CircularBuffer(TrackAudioPlayer* track1, TrackAudioPlayer* track2) : source1(track1), source2(track2)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

CircularBuffer::~CircularBuffer()
{
}

void CircularBuffer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    auto delayBufferSize = sampleRate * delayTime;
    delayBuffer.setSize(2, (int)delayBufferSize);
}

void CircularBuffer::releaseResources() {

}

void CircularBuffer::setDelayTime(float newTime) {
    //figure out how to do this properly
    delayTime = newTime;
}

void CircularBuffer::setDelayGain(float newGain) {
    delayGain = newGain;
}

void CircularBuffer::setDelayStatus(bool newStatus) {
    delayStatus = newStatus;
}


void CircularBuffer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {

    juce::ScopedNoDenormals noDenormals;

        for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
            bufferToFill.buffer->clear(i, 0, bufferToFill.buffer->getNumSamples());
        }

        auto bufferSize = bufferToFill.buffer->getNumSamples();
        auto delayBufferSize = delayBuffer.getNumSamples();

        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            auto* channelData = bufferToFill.buffer->getWritePointer(channel);

            //fills the circular buffer
            fillBuffer(channel, bufferSize, delayBufferSize, channelData);
            //adds to the main buffer
            if (delayStatus && (source1->getPlayingState() || source2->getPlayingState())) {
                readFromBuffer(channel, bufferSize, delayBufferSize, bufferToFill, delayBuffer);
                fillBuffer(channel, bufferSize, delayBufferSize, channelData);
            }
        }
    

    writePosition += bufferSize;
    writePosition %= delayBufferSize;
}

void CircularBuffer::readFromBuffer(int channel, int bufferSize, int delayBufferSize, const juce::AudioSourceChannelInfo& bufferToFill, juce::AudioBuffer<float>& delayBuffer) {
    auto readPosition = writePosition - 44100.0;

    if (readPosition < 0) {
        readPosition += delayBufferSize;
    }

    float gain = 1.f;

    if (readPosition + bufferSize < delayBufferSize) {
        bufferToFill.buffer->addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), bufferSize, delayGain, delayGain);
    }
    else {
        auto numSamplesRemaining = delayBufferSize - readPosition;
        bufferToFill.buffer->addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), numSamplesRemaining, delayGain, delayGain);

        auto numSamplesAtStart = bufferSize - numSamplesRemaining;
        bufferToFill.buffer->addFromWithRamp(channel, numSamplesRemaining, delayBuffer.getReadPointer(channel, 0), numSamplesAtStart, delayGain, delayGain);
    }
}

void CircularBuffer::fillBuffer(int channel, int bufferSize, int delayBufferSize, float* channelData) {
    if (delayBufferSize >= bufferSize + writePosition) {
        delayBuffer.copyFrom(channel, writePosition, channelData, bufferSize);
    }
    else {
        // Figure out how much space is left at the end of the buffer
        auto numSamplesRemaining = delayBufferSize - writePosition;

        delayBuffer.copyFrom(channel, writePosition, channelData, numSamplesRemaining);

        auto numSamplesAtStart = bufferSize - numSamplesRemaining;
        // Copy remaining samples to the start (start of the "loop"/"circle")
        delayBuffer.copyFrom(channel, 0, channelData + numSamplesRemaining, numSamplesAtStart);
    }
}