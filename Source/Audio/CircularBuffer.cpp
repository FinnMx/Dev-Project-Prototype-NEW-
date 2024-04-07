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
CircularBuffer::CircularBuffer()
{

    lowFilterCoefficient = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, lowFrequencyBand, 1.0f);
    *lowFilter.state = lowFilterCoefficient;
    highFilterCoefficient = *juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, highFrequencyBand, 1.0f);
    *highFilter.state = highFilterCoefficient;
}

CircularBuffer::~CircularBuffer()
{
}

void CircularBuffer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {

    copyBuffer.clear();

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 44100.0;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = 2;

    lowFilter.prepare(spec);
    highFilter.prepare(spec);

    for (auto& smooth : smoother)
        smooth.reset(spec.sampleRate, 0.5f);

    delay.reset();
    delay.prepare(spec);
    linear.reset();
    linear.prepare(spec);

    testFilterHighL.setCoefficients(juce::IIRCoefficients::makeHighPass(44100, 1500));
    testFilterLowL.setCoefficients(juce::IIRCoefficients::makeLowPass(44100, 750));
    testFilterHighR.setCoefficients(juce::IIRCoefficients::makeHighPass(44100, 1500));
    testFilterLowR.setCoefficients(juce::IIRCoefficients::makeLowPass(44100, 750));

    for (auto& volume : delayFeedbackVolume)
        volume.reset(spec.sampleRate, 0.05);

    linear.reset();
    std::fill(lastDelayOutput.begin(), lastDelayOutput.end(), 0.0f);
}

void CircularBuffer::releaseResources() {

}

void CircularBuffer::setDelayTime(float newTime) {
    if (newTime >= 0) {
        for (auto& smooth : smoother)
            smooth.setTargetValue(newTime / 1000.0 * 44100.f);
    }

}

void CircularBuffer::setDelayCutoffFrequency(float newFrequency) {
    lowFrequencyBand = newFrequency + 750.f;
    highFrequencyBand = newFrequency - 750.f;
    lowFilterCoefficient = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, lowFrequencyBand, 1.0f);
    *lowFilter.state = lowFilterCoefficient;
    highFilterCoefficient = *juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, highFrequencyBand, 1.0f);
    *highFilter.state = highFilterCoefficient;
    testFilterHighL.setCoefficients(juce::IIRCoefficients::makeHighPass(44100, highFrequencyBand));
    testFilterLowL.setCoefficients(juce::IIRCoefficients::makeLowPass(44100, lowFrequencyBand));
    testFilterHighR.setCoefficients(juce::IIRCoefficients::makeHighPass(44100, highFrequencyBand));
    testFilterLowR.setCoefficients(juce::IIRCoefficients::makeLowPass(44100, lowFrequencyBand));
}

void CircularBuffer::setDelayLowCutoffFrequency(float newFrequencyCutoff) {
    lowFilterCoefficient = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, newFrequencyCutoff, 1.0f);
    *lowFilter.state = lowFilterCoefficient;
    testFilterLowL.setCoefficients(juce::IIRCoefficients::makeLowPass(44100, newFrequencyCutoff));
    testFilterLowR.setCoefficients(juce::IIRCoefficients::makeLowPass(44100, newFrequencyCutoff));
}

void CircularBuffer::setDelayHighCutoffFrequency(float newFrequencyCutoff) {
    highFilterCoefficient = *juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, newFrequencyCutoff, 1.0f);
    *highFilter.state = highFilterCoefficient;
    testFilterHighL.setCoefficients(juce::IIRCoefficients::makeHighPass(44100, newFrequencyCutoff));
    testFilterHighR.setCoefficients(juce::IIRCoefficients::makeHighPass(44100, newFrequencyCutoff));
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

std::vector<juce::dsp::IIR::Coefficients<float>*> CircularBuffer::getCoefficients() {
    std::vector<juce::dsp::IIR::Coefficients<float>*> tempVec;

    tempVec.push_back(&lowFilterCoefficient);
    tempVec.push_back(&highFilterCoefficient);

    return tempVec;
}


void CircularBuffer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    juce::ScopedNoDenormals noDenormals;

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        bufferToFill.buffer->clear(i, 0, bufferToFill.buffer->getNumSamples());
    
    const auto numChannels = juce::jmax(totalNumInputChannels, totalNumOutputChannels);

    copyBuffer.setSize(2, bufferToFill.buffer->getNumSamples());
    auto* data = bufferToFill.buffer->getReadPointer(0);
    copyBuffer.copyFrom(0, 0, data, bufferToFill.buffer->getNumSamples());
    copyBuffer.copyFrom(1, 0, data, bufferToFill.buffer->getNumSamples());

    copyBuffer.applyGain(1.f);

    auto audioBlock = juce::dsp::AudioBlock<float>(copyBuffer).getSubsetChannelBlock(0, (size_t)numChannels);
    auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);

    const auto& input = context.getInputBlock();
    const auto& output = context.getOutputBlock();


    for (size_t channel = 0; channel < numChannels; ++channel)
    {

        auto* samplesIn = input.getChannelPointer(channel);
        auto* samplesOut = output.getChannelPointer(channel);

            for (int sample = 0; sample < input.getNumSamples(); ++sample)
            {

                auto smoothedVal = smoother[channel].getNextValue() * coef;

                if (delayStatus) {
                    auto input = samplesIn[sample] - lastDelayOutput[channel];
                    linear.pushSample(int(channel), input);
                }
                else {
                    auto input = lastDelayOutput[channel];
                    linear.pushSample(int(channel), input);
                }

                samplesOut[sample] = linear.popSample((int)channel, smoothedVal); 

                lastDelayOutput[channel] = samplesOut[sample] * delayFeedbackVolume[channel].getNextValue();;
            }
       
            //Apply frequency band to the repeating samples 
            if(channel == 0){
            testFilterLowL.processSamples(samplesOut, bufferToFill.buffer->getNumSamples());
            testFilterHighL.processSamples(samplesOut, bufferToFill.buffer->getNumSamples());
            }
            else {
                testFilterLowR.processSamples(samplesOut, bufferToFill.buffer->getNumSamples());
                testFilterHighR.processSamples(samplesOut, bufferToFill.buffer->getNumSamples());
            }
           
            //windower.multiplyWithWindowingTable(samplesOut, 480);
            bufferToFill.buffer->addFrom(channel, 0, samplesOut, bufferToFill.buffer->getNumSamples(), 1.0f);
    }
}