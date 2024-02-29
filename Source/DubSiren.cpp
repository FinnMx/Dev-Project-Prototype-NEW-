/*
  ==============================================================================

    DubSiren.cpp
    Created: 29 Feb 2024 3:10:56pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DubSiren.h"
#include <cmath>

//==============================================================================
DubSiren::DubSiren() 
{
    oscillator.initialise([](float x) {return  (x < 0.0f) ? -1.0f : 1.0f; });
    oscillator.setFrequency(frequency);

    lfoOscillator.initialise([](float x) { return std::sin(x * juce::MathConstants<float>::twoPi); });
    lfoOscillator.setFrequency(lfoFrequency);
}

DubSiren::~DubSiren()
{
}

void DubSiren::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    updateAngleDelta();

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 44100.0;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = 2;

    oscillator.prepare(spec);
    lfoOscillator.prepare(spec);
    filter.setCoefficients(juce::IIRCoefficients::makeHighPass(44100.f, 500.f, 1.f));
}

void DubSiren::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    juce::ScopedNoDenormals noDenormals;

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        bufferToFill.buffer->clear(i, 0, bufferToFill.buffer->getNumSamples());

    const auto numChannels = juce::jmax(totalNumInputChannels, totalNumOutputChannels);

    if (trigger) {

        copyBuffer.setSize(2, bufferToFill.buffer->getNumSamples());
        auto* data = bufferToFill.buffer->getReadPointer(0);
        copyBuffer.copyFrom(0, 0, data, bufferToFill.buffer->getNumSamples());
        copyBuffer.copyFrom(1, 0, data, bufferToFill.buffer->getNumSamples());

        auto level = 0.125f;
        auto* leftBuffer = copyBuffer.getWritePointer(0, bufferToFill.startSample);
        auto* rightBuffer = copyBuffer.getWritePointer(1, bufferToFill.startSample);

        for (auto sample = 0; sample < copyBuffer.getNumSamples(); ++sample)
        {
            lfoOscillator.setFrequency(lfoFrequency);
            auto lfoValue = lfoOscillator.processSample(0.0);
            oscillator.setFrequency(frequency + (lfoValue * 60.f)); // 0.7 IS THE "MODULATION DEPTH"

            auto currentSample = oscillator.processSample(0.0); 
            leftBuffer[sample] = currentSample * level;
            rightBuffer[sample] = currentSample * level;
        }

        filter.processSamples(leftBuffer, 480);
        filter.processSamples(rightBuffer, 480);

        //LAST FLOAT SHOULD BE A VOLUME VALUE
        bufferToFill.buffer->addFrom(0, 0, leftBuffer, 480, volume);
        bufferToFill.buffer->addFrom(1, 0, rightBuffer, 480, volume);
    }
}

void DubSiren::setTrigger(bool newTrigger) {
    trigger = newTrigger;
}

void DubSiren::setFrequency(float newFrequency) {
    frequency = newFrequency;
}

void DubSiren::setLfoFrequency(float newLfoFrequency) {
    lfoFrequency = newLfoFrequency;
}

void DubSiren::setModulationDepth(float newModulationDepth) {
    modulationDepth = newModulationDepth;
}

void DubSiren::setVolume(float newVolume) {
    volume = newVolume;
}

void DubSiren::releaseResources() {

}

void DubSiren::updateAngleDelta() {
}
