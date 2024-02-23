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
    positionDifference = delayTime - newTime;
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

    //usually 480
    auto bufferSize = bufferToFill.buffer->getNumSamples();

    //the size of our delay buffer for storing samples from the past, anything after the incoming sample rate size (usually 44100) is "past audio"
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

    delayBuffer.setSize(2, (int)(44100.0 * delayTime), true);
}

void CircularBuffer::readFromBuffer(int channel, int bufferSize, int delayBufferSize, const juce::AudioSourceChannelInfo& bufferToFill, juce::AudioBuffer<float>& delayBuffer) {
    auto readPosition = writePosition - 44100.0;

    if (readPosition < 0) {
        readPosition += delayBufferSize;
    }

    if (readPosition + bufferSize < delayBufferSize) {
        //THIS NEEDS A CLOSER LOOKAT, IT IS RESPONSIBLE FOR THE READING OF THE DELAY BUFFER AND CAN FUCK UP WHEN THE TIME IS CHANGED, IF THE DELAYBUFFER DOES NOT KEEP PREVIOUS VALS IT WILL TRY AND READ SHIT DATA
        //DBG("error val: " << *(delayBuffer.getReadPointer(channel, readPosition)));
        const float* sample = delayBuffer.getReadPointer(channel, readPosition);
        //const float* diffSample = delayBuffer.getReadPointer(channel, readPosition + positionDifference);
        if (abs(*sample)) {
            DBG("sample" << *sample);
            bufferToFill.buffer->addFromWithRamp(channel, 0, sample, bufferSize, delayGain, delayGain);
        }
        //else {
            //DBG("diff sample: " << *diffSample);
            //bufferToFill.buffer->addFromWithRamp(channel, 0, diffSample , bufferSize, delayGain, delayGain);
        //}
    }
    else {
        auto numSamplesRemaining = delayBufferSize - readPosition;
        bufferToFill.buffer->addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), numSamplesRemaining, delayGain, delayGain);

        auto numSamplesAtStart = bufferSize - numSamplesRemaining;
        bufferToFill.buffer->addFromWithRamp(channel, numSamplesRemaining, delayBuffer.getReadPointer(channel, 0), numSamplesAtStart, delayGain, delayGain);
    }
}

void CircularBuffer::fillBuffer(int channel, int bufferSize, int delayBufferSize, float* channelData) {
    //if we are not about to go over the delay buffer size then fill it with the buffer chunk
    if (delayBufferSize >= bufferSize + writePosition) {
        delayBuffer.copyFrom(channel, writePosition, channelData, bufferSize);
    }
    else {
        // We are going over so we need to figure out how much space is left that we can fill from our buffer
        auto numSamplesRemaining = abs(delayBufferSize - writePosition);

        //DBG("samples remaining: " << numSamplesRemaining);

        if (writePosition < delayBufferSize) {
            //fills the delay buffer completley
            delayBuffer.copyFrom(channel, writePosition, channelData, numSamplesRemaining);
        }

        //gets the size of how many samples we couldnt fill
        auto numSamplesAtStart = abs(bufferSize - numSamplesRemaining);

        //DBG("samples at start: " << numSamplesAtStart);
        // Copy remaining samples to the start (start of the buffer loop)
        delayBuffer.copyFrom(channel, 0, channelData + numSamplesRemaining, numSamplesAtStart);
    }
}