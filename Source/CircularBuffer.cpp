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
CircularBuffer::CircularBuffer()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

CircularBuffer::~CircularBuffer()
{
}

void CircularBuffer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    auto delayBufferSize = sampleRate * 2.0;
    delayBuffer.setSize(2, (int)delayBufferSize);
}

void CircularBuffer::releaseResources() {

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

        auto readPosition = writePosition - 44100.0;

        if (readPosition < 0) {
            readPosition += delayBufferSize;
        }

        float gain = 2.f;

        if (readPosition + bufferSize < delayBufferSize) {
            bufferToFill.buffer->addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), bufferSize, gain, gain);
        }
        else {
            auto numSamplesRemaining = delayBufferSize - readPosition;
            bufferToFill.buffer->addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), numSamplesRemaining, gain, gain);

            auto numSamplesAtStart = bufferSize - numSamplesRemaining;
            bufferToFill.buffer->addFromWithRamp(channel, numSamplesRemaining, delayBuffer.getReadPointer(channel, 0), numSamplesAtStart, gain, gain);
        }
    }

    DBG("delay buffer size:" << delayBufferSize);
    DBG("buffer size:" << bufferSize);
    DBG("Write Pos:" << writePosition);
   

    writePosition += bufferSize;
    writePosition %= delayBufferSize;
}

void CircularBuffer::fillBuffer(int channel, int bufferSize, int delayBufferSize, float* channelData) {
    if (delayBufferSize > bufferSize + writePosition) {
        delayBuffer.copyFromWithRamp(channel, writePosition, channelData, bufferSize, 0.1f, 0.1f);
    }
    else {
        // Figure out how much space is left at the end of the buffer
        auto numSamplesRemaining = delayBufferSize - writePosition;

        delayBuffer.copyFromWithRamp(channel, writePosition, channelData, numSamplesRemaining, 0.1f, 0.1f);

        auto numSamplesAtStart = bufferSize - numSamplesRemaining;
        // Copy remaining samples to the start (start of the "loop"/"circle")
        delayBuffer.copyFromWithRamp(channel, 0, channelData + numSamplesRemaining, numSamplesAtStart, 0.1f, 0.1f);
    }
}