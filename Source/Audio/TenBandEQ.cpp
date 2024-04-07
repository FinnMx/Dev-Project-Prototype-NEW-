/*
  ==============================================================================

    TenBandEQ.cpp
    Created: 6 Mar 2024 3:47:02pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TenBandEQ.h"

//==============================================================================
TenBandEQ::TenBandEQ()
{
    for each (float freq in frequencies)
    {
        juce::dsp::IIR::Coefficients<float> tempCof;
        tempCof = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(44100.f, freq, 1.f, 1.0f);
        coefficients.push_back(tempCof);
    }

    filters.push_back(&freq30);
    filters.push_back(&freq62);
    filters.push_back(&freq125);
    filters.push_back(&freq250);
    filters.push_back(&freq500);
    filters.push_back(&freq1000);
    filters.push_back(&freq2000);
    filters.push_back(&freq4000);
    filters.push_back(&freq8000);
    filters.push_back(&freq16000);

    for (int i = 0; i <= 9; i++) {
        *filters[i]->state = coefficients[i];
    }
}

TenBandEQ::~TenBandEQ()
{
}

void TenBandEQ::setNewGain(int sliderNum, float newGain) {
    juce::dsp::IIR::Coefficients<float> tempCof;
    tempCof = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(44100.f, frequencies[sliderNum], 1.f, newGain);
    *filters[sliderNum]->state = tempCof;
}

void TenBandEQ::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 44100.f;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = totalNumOutputChannels;

    for each (juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>* filter in filters)
    {
        filter->prepare(spec);
    }

}

void TenBandEQ::releaseResources() {

}

void TenBandEQ::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    juce::ScopedNoDenormals noDenormals;

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        bufferToFill.buffer->clear(i, 0, bufferToFill.buffer->getNumSamples());

    juce::dsp::AudioBlock<float> block(*bufferToFill.buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    for each (juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>*filter in filters)
    {
        filter->process(context);
    }

    frequencyLevelThread.updateBuffer(*bufferToFill.buffer);
}

void TenBandEQ::calculateFrequencyBandRMS() {

}


void TenBandEQ::autoAdjustFrequencies(std::vector<juce::Slider*> &sliders) {
    frequencyLevelThread.setSliders(sliders);
    frequencyLevelThread.setTargetRMSValues(targetRMSValues);
    frequencyLevelThread.startThread();
}
