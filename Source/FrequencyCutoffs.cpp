/*
  ==============================================================================

    FrequencyCutoffs.cpp
    Created: 27 Feb 2024 1:16:27pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FrequencyCutoffs.h"

//==============================================================================
FrequencyCutoffs::FrequencyCutoffs() : subBassFilter(juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, 95.f, 1)),
bassFilter(juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, 250.f, 1)),
midsFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, 3500.f, 1)),
highFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, 5000.f, 1))
{

}

FrequencyCutoffs::~FrequencyCutoffs()
{
}

void FrequencyCutoffs::setSubBassFilter(float newFreq) {
    *subBassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, newFreq, 1);
}

void FrequencyCutoffs::setbassFilterr(float newFreq) {
    *bassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, newFreq, 0.7);
}

void FrequencyCutoffs::setmidsFilter(float newFreq) {
    *midsFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, newFreq, 1);
}

void FrequencyCutoffs::sethighFilter(float newFreq) {
    *highFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, newFreq, 1);
}

void FrequencyCutoffs::setSubBassStatus(bool newsubBassStatus) {
    if(!newsubBassStatus)
        subBassSmoother.setTargetValue(0.f);
    subBassStatus = newsubBassStatus;
}
void FrequencyCutoffs::setBassStatus(bool newbassStatus) {
    if (!newbassStatus)
        bassSmoother.setTargetValue(0.f);
    bassStatus = newbassStatus;
}
void FrequencyCutoffs::setMidsStatus(bool newmidsStatus) {
    if (!newmidsStatus)
        midsSmoother.setTargetValue(0.f);
    midsStatus = newmidsStatus;
}
void FrequencyCutoffs::setHighStatus(bool newhighStatus) {
    if (!newhighStatus)
        highSmoother.setTargetValue(0.f);
    highStatus = newhighStatus;
}



void FrequencyCutoffs::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 44100.f;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = totalNumOutputChannels;

    subBassSmoother.reset(spec.sampleRate, 0.1);
    bassSmoother.reset(spec.sampleRate, 0.1);
    midsSmoother.reset(spec.sampleRate, 0.1);
    highSmoother.reset(spec.sampleRate, 0.1);

    subBassFilter.prepare(spec);
    bassFilter.prepare(spec);
    midsFilter.prepare(spec);
    highFilter.prepare(spec);
    //subBassFilter.makeInactive();
    //bassFilter.makeInactive();
    //midsFilter.makeInactive();
    //highFilter.makeInactive();

}

void FrequencyCutoffs::releaseResources() {

}

void FrequencyCutoffs::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    juce::ScopedNoDenormals noDenormals;

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        bufferToFill.buffer->clear(i, 0, bufferToFill.buffer->getNumSamples());

    juce::dsp::AudioBlock<float> block(*bufferToFill.buffer);

    switch (subBassStatus) {
        case true:
            subBassSmoother.setTargetValue(subBassFreq);
            subBassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
            break;
        default:
            break;
    }
    switch (bassStatus) {
    case true:
        bassSmoother.setTargetValue(bassFreq);
        bassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
        break;
    default:
        break;
    }
    switch (midsStatus) {
    case true:
        midsSmoother.setTargetValue(midsFreq);
        midsFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
        break;
    default:
        break;
    }
    switch (highStatus) {
    case true:
        highSmoother.setTargetValue(highFreq);
        highFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
        break;
    default:
        break;
    }

}