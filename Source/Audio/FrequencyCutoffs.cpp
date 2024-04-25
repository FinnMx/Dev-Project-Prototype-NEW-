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
FrequencyCutoffs::FrequencyCutoffs()
{
    subBassCoefficient = *juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, 95.f, 1);
    bassCoefficient = *juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, 250.f, 1);
    midsCoefficient = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, 3500.f, 1);
    highCoefficient = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, 5000.f, 1);
    *subBassFilter.state = subBassCoefficient;
    *bassFilter.state = bassCoefficient;
    *midsFilter.state = midsCoefficient;
    *highFilter.state = highCoefficient;
}

FrequencyCutoffs::~FrequencyCutoffs()
{
}

void FrequencyCutoffs::setSubBassFilter(float newFreq) {
    if (newFreq == 0)
        return;
    subBassCoefficient = *juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, newFreq, 0.7);
    *subBassFilter.state = subBassCoefficient;
}

void FrequencyCutoffs::setbassFilterr(float newFreq) {
    if (newFreq == 0)
        return;
    bassCoefficient = *juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, newFreq, 0.7);
    *bassFilter.state = bassCoefficient;
}

void FrequencyCutoffs::setmidsFilter(float newFreq) {
    if (newFreq == 0)
        return;
    midsCoefficient = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, newFreq, 0.7);
    *midsFilter.state = midsCoefficient;
}

void FrequencyCutoffs::sethighFilter(float newFreq) {
    if (newFreq == 0)
        return;
    highCoefficient = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, newFreq, 0.7);
    *highFilter.state = highCoefficient;
}

void FrequencyCutoffs::setSubBassStatus(bool newsubBassStatus) {
    subBassStatus = newsubBassStatus;

}
void FrequencyCutoffs::setBassStatus(bool newbassStatus) {
    bassStatus = newbassStatus;

}
void FrequencyCutoffs::setMidsStatus(bool newmidsStatus) {
    midsStatus = newmidsStatus;
}
void FrequencyCutoffs::setHighStatus(bool newhighStatus) {
    highStatus = newhighStatus;
}

void FrequencyCutoffs::subBassTarget(float newFreq) {
    subBassFreq = newFreq;
}

void FrequencyCutoffs::bassTarget(float newFreq) {
    bassFreq = newFreq;
}

void FrequencyCutoffs::midsTarget(float newFreq) {
    midsFreq = newFreq;
}

void FrequencyCutoffs::highTarget(float newFreq) {
    highFreq = newFreq;
}



void FrequencyCutoffs::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 44100.f;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = totalNumOutputChannels;

    subBassSmoother.reset(spec.sampleRate, 0.00005);
    bassSmoother.reset(spec.sampleRate, 0.00005);
    midsSmoother.reset(spec.sampleRate, 0.00005);
    highSmoother.reset(spec.sampleRate, 0.00005);

    subBassFilter.prepare(spec);
    bassFilter.prepare(spec);
    midsFilter.prepare(spec);
    highFilter.prepare(spec);

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
        setSubBassFilter(subBassSmoother.getNextValue());
        subBassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
        break;
    case false:
        subBassSmoother.setTargetValue(20.f);
        setSubBassFilter(subBassSmoother.getNextValue());
        break;
    }

    switch (bassStatus) {
    case true:
        bassSmoother.setTargetValue(bassFreq);
        setbassFilterr(bassSmoother.getNextValue());
        bassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
        break;
    case false:
        bassSmoother.setTargetValue(20.f);
        setbassFilterr(bassSmoother.getNextValue());
        break;
    }

    switch (midsStatus) {
    case true:
        midsSmoother.setTargetValue(midsFreq);
        setmidsFilter(midsSmoother.getNextValue());
        midsFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
        break;
    case false:
        midsSmoother.setTargetValue(20000.f);
        setmidsFilter(midsSmoother.getNextValue());
        break;
    }

    switch (highStatus) {
    case true:
        highSmoother.setTargetValue(highFreq);
        sethighFilter(highSmoother.getNextValue());
        highFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
        break;
    case false:
        highSmoother.setTargetValue(20000.f);
        sethighFilter(highSmoother.getNextValue());
        break;
    }

}

std::vector<juce::dsp::IIR::Coefficients<float>*> FrequencyCutoffs::getCoefficients() {
    std::vector<juce::dsp::IIR::Coefficients<float>*> tempVec;

    tempVec.push_back(&subBassCoefficient);
    tempVec.push_back(&bassCoefficient);
    tempVec.push_back(&midsCoefficient);
    tempVec.push_back(&highCoefficient);

    return tempVec;
}