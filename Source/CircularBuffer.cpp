/*
  ==============================================================================

    CircularBuffer.cpp
    Created: 22 Feb 2024 11:01:45pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CircularBuffer.h"
#include <random>

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
    //auto delayBufferSize = sampleRate * delayTime;
    //delayBuffer.setSize(2, (int)delayBufferSize, true, true);

    //monoFilter.setCoefficients(juce::IIRCoefficients::makeHighPass(44100.f, 350, 1));

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 44100.0;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = 2;

    delay.reset();
    delay.prepare(spec);
    linear.prepare(spec);
    mixer.prepare(spec);

    for (auto& volume : delayFeedbackVolume)
        volume.reset(spec.sampleRate, 0.05);

    linear.reset();
    std::fill(lastDelayOutput.begin(), lastDelayOutput.end(), 0.0f);
    mixer.setWetMixProportion(1.f);

}

void CircularBuffer::releaseResources() {

}

void CircularBuffer::setDelayTime(float newTime) {
    auto time = std::round(newTime / 1000.0 * 44100.f);
    std::fill(delayValue.begin(), delayValue.end(), time);
}

void CircularBuffer::setDelayFeedback(float newFeedback) {
    delayFeedback = newFeedback;

    const auto feedbackGain = juce::Decibels::decibelsToGain(delayFeedback, -100.0f);

    for (auto& volume : delayFeedbackVolume)
        volume.setTargetValue(delayFeedback);
}

void CircularBuffer::setDelayStatus(bool newStatus) {
    delayStatus = newStatus;
}


void CircularBuffer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    juce::ScopedNoDenormals noDenormals;

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        bufferToFill.buffer->clear(i, 0, bufferToFill.buffer->getNumSamples());

    const auto numChannels = juce::jmax(totalNumInputChannels, totalNumOutputChannels);

    auto audioBlock = juce::dsp::AudioBlock<float>(*bufferToFill.buffer).getSubsetChannelBlock(0, (size_t)numChannels);
    auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);
    
    const auto& input = context.getInputBlock();
    const auto& output = context.getOutputBlock();

    mixer.pushDrySamples(input);

    for (size_t channel = 0; channel < numChannels; ++channel)
    {
        auto* samplesIn = input.getChannelPointer(channel);
        auto* samplesOut = output.getChannelPointer(channel);

        if (delayStatus) {

            for (size_t sample = 0; sample < input.getNumSamples(); ++sample)
            {
                // NOT ACTUALLY TOO SURE IF DENORMALISATION IS NEEDED? BUT I THINK THERES LESS POPPING AND CLICKING
                // WHEN THE FLOAT SAMPLES ARE DENORMALISED?!?!? MAYBE IM GOING CRAZY.

                auto input = samplesIn[sample] - lastDelayOutput[channel];

                auto delayAmount = delayValue[channel];
                JUCE_UNDENORMALISE(input);
                JUCE_UNDENORMALISE(delayAmount);

                linear.pushSample(int(channel), input);
                linear.setDelay((float)delayAmount);
                samplesOut[sample] = linear.popSample((int)channel);
                JUCE_UNDENORMALISE(samplesOut[sample]);

                lastDelayOutput[channel] = samplesOut[sample] * delayFeedbackVolume[channel].getNextValue();;

                JUCE_UNDENORMALISE(lastDelayOutput[channel]);
                //DBG(sample);
            }
        }
    }

    mixer.mixWetSamples(output);
}

/*
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
            //Stretcher.processBuffer(bufferToFill.buffer);
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

*/