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
    smoother.reset(44100.f, 2.f);

    delay.reset();
    delay.prepare(spec);
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
    if(newTime >= 0)
        smoother.setTargetValue(newTime / 1000.0 * 44100.f);
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

    //lowFilter.process(context);
    //highFilter.process(context);

    const auto& input = context.getInputBlock();
    const auto& output = context.getOutputBlock();

    for (size_t channel = 0; channel < numChannels; ++channel)
    {

        auto* samplesIn = input.getChannelPointer(channel);
        auto* samplesOut = output.getChannelPointer(channel);

            for (size_t sample = 0; sample < input.getNumSamples(); ++sample)
            {
                //creates the "loop"
                if (delayStatus) {
                    auto input = samplesIn[sample] - lastDelayOutput[channel];
                    linear.pushSample(int(channel), input);
                }
                else {
                    auto input = lastDelayOutput[channel];
                    linear.pushSample(int(channel), input);
                }

                auto smoothedVal = smoother.getNextValue();
                if(smoothedVal > 0 )
                    linear.setDelay(smoothedVal);

                samplesOut[sample] = linear.popSample((int)channel);

                //SAMPLESOUT[OUT] IS THE RAW SAMPLE IN
                lastDelayOutput[channel] = samplesOut[sample] * delayFeedbackVolume[channel].getNextValue();;
            }

       
            //Apply frequency band to the repeating samples 
            if(channel == 0){
            testFilterLowL.processSamples(samplesOut, 480);
            testFilterHighL.processSamples(samplesOut, 480);
            }
            else {
                testFilterLowR.processSamples(samplesOut, 480);
                testFilterHighR.processSamples(samplesOut, 480);
            }
           
            //windower.multiplyWithWindowingTable(samplesOut, 480);
            bufferToFill.buffer->addFrom(channel, 0, samplesOut, 480, 1.0f);
    }
}