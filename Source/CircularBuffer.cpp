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
CircularBuffer::CircularBuffer() : bandFilter(juce::dsp::IIR::Coefficients<float>::makeBandPass(44100.f, frequencyBand, 0.5f))
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
    //filter.setCoefficients(juce::IIRCoefficients::makeHighPass(44100.f, 2000.f, 1));
    //filter.setCoefficients(juce::IIRCoefficients::makeLowPass(44100.f, 10000.f, 1));

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 44100.0;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = 2;

    bandFilter.prepare(spec);
    smoother.reset(44100.f, 2.f);

    delay.reset();
    delay.prepare(spec);
    linear.prepare(spec);

    for (auto& volume : delayFeedbackVolume)
        volume.reset(spec.sampleRate, 0.05);

    linear.reset();
    std::fill(lastDelayOutput.begin(), lastDelayOutput.end(), 0.0f);
}

void CircularBuffer::releaseResources() {

}

void CircularBuffer::setDelayTime(float newTime) {
    if(newTime >= 0)
        smoother.setTargetValue(newTime / 1000.0 * 44100.f);
    //auto time = std::round(newTime / 1000.0 * 44100.f);
    //std::fill(delayValue.begin(), delayValue.end(), time);
}

void CircularBuffer::setDelayCutoffFrequency(float newFrequency) {
    frequencyBand = newFrequency;
    *bandFilter.state = *juce::dsp::IIR::Coefficients<float>::makeBandPass(44100.f, newFrequency, 0.5f);
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

    /*
    auto audioBlock = juce::dsp::AudioBlock<float>(*bufferToFill.buffer).getSubsetChannelBlock(0, (size_t)numChannels);
    auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);
    
    const auto& input = context.getInputBlock();
    const auto& output = context.getOutputBlock();
    */

    copyBuffer.setSize(2, bufferToFill.buffer->getNumSamples());
    auto* data = bufferToFill.buffer->getReadPointer(0);
    copyBuffer.copyFrom(0, 0, data, bufferToFill.buffer->getNumSamples());
    copyBuffer.copyFrom(1, 0, data, bufferToFill.buffer->getNumSamples());

    copyBuffer.applyGain(3.f);

    auto audioBlock = juce::dsp::AudioBlock<float>(copyBuffer).getSubsetChannelBlock(0, (size_t)numChannels);
    auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);

    bandFilter.process(context);

    const auto& input = context.getInputBlock();
    const auto& output = context.getOutputBlock();

    for (size_t channel = 0; channel < numChannels; ++channel)
    {
        auto* samplesIn = input.getChannelPointer(channel);
        auto* samplesOut = output.getChannelPointer(channel);

            for (size_t sample = 0; sample < input.getNumSamples(); ++sample)
            {
                
                if (delayStatus) {
                    auto input = samplesIn[sample] - lastDelayOutput[channel];
                    linear.pushSample(int(channel), input);
                }
                else {
                    auto input = lastDelayOutput[channel];
                    linear.pushSample(int(channel), input);
                }

                linear.setDelay(smoother.getNextValue());
                samplesOut[sample] = linear.popSample((int)channel);

                //SAMPLESOUT[OUT] IS THE RAW SAMPLE IN
                lastDelayOutput[channel] = samplesOut[sample] * delayFeedbackVolume[channel].getNextValue();;
            }

            //windower.multiplyWithWindowingTable(samplesOut, 480);
            bufferToFill.buffer->addFrom(channel, 0, samplesOut, 480, 0.3f);
    }
}