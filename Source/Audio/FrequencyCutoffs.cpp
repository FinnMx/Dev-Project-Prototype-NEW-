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
    subBassCoefficient = *juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, 20.f, 1);
    bassCoefficient = *juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, 20.f, 1);
    midsCoefficient = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, 20000.f, 1);
    highCoefficient = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, 20000.f, 1);
    *subBassFilter.state = subBassCoefficient;
    *bassFilter.state = bassCoefficient;
    *midsFilter.state = midsCoefficient;
    *highFilter.state = highCoefficient;
}

FrequencyCutoffs::~FrequencyCutoffs()
{
}

void FrequencyCutoffs::setSubBassFilter(float newFreq) {
    if (newFreq == 20.f) {
        subBassCoefficient = *juce::dsp::IIR::Coefficients<float>::makeAllPass(44100, 20);
        *subBassFilter.state = subBassCoefficient;
    }
    subBassCoefficient = *juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, newFreq, 0.7);
    *subBassFilter.state = subBassCoefficient;
}

void FrequencyCutoffs::setbassFilterr(float newFreq) {
    if (newFreq == 20.f) {
        bassCoefficient = *juce::dsp::IIR::Coefficients<float>::makeAllPass(44100, 20);
        *bassFilter.state = bassCoefficient;
    }
    bassCoefficient = *juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, newFreq, 0.7);
    *bassFilter.state = bassCoefficient;
}

void FrequencyCutoffs::setmidsFilter(float newFreq) {
    if (newFreq == 20000.f) {
        midsCoefficient = *juce::dsp::IIR::Coefficients<float>::makeAllPass(44100, 20000);
        *midsFilter.state = midsCoefficient;
    }
    midsCoefficient = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, newFreq, 0.7);
    *midsFilter.state = midsCoefficient;
}

void FrequencyCutoffs::sethighFilter(float newFreq) {
    if (newFreq == 20000.f) {
        highCoefficient = *juce::dsp::IIR::Coefficients<float>::makeAllPass(44100, 20000);
        *highFilter.state = highCoefficient;
    }
    highCoefficient = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, newFreq, 0.7);
    *highFilter.state = highCoefficient;
}

void FrequencyCutoffs::setNewSubBassFreq(float newFreq) {
    subBassFreq = newFreq;
    if(subBassStatus)
        setSubBassFilter(subBassFreq);
}

void FrequencyCutoffs::setNewBassFreq(float newFreq) {
    bassFreq = newFreq;
    if(bassStatus)
        setbassFilterr(bassFreq);
}

void FrequencyCutoffs::setNewMidsFreq(float newFreq) {
    midsFreq = newFreq;
    if(midsStatus)
        setmidsFilter(midsFreq);
}

void FrequencyCutoffs::setNewHighFreq(float newFreq) {
    highFreq = newFreq;
    if(highStatus)
        sethighFilter(highFreq);
}

void FrequencyCutoffs::setSubBassStatus(bool newsubBassStatus) {
    subBassStatus = newsubBassStatus;
    if (!newsubBassStatus)
        setSubBassFilter(20.f);
    if (newsubBassStatus) {
        setSubBassFilter(subBassFreq);
    }
}
void FrequencyCutoffs::setBassStatus(bool newbassStatus) {
    bassStatus = newbassStatus;
    if (!newbassStatus)
        setbassFilterr(20.f);
    if (newbassStatus) {
        setbassFilterr(bassFreq);
    }
}
void FrequencyCutoffs::setMidsStatus(bool newmidsStatus) {
    midsStatus = newmidsStatus;
    if (!newmidsStatus)
        setmidsFilter(20000.f);
    if (newmidsStatus) {
        setmidsFilter(midsFreq);
    }
}
void FrequencyCutoffs::setHighStatus(bool newhighStatus) {
    highStatus = newhighStatus;
    if (!newhighStatus)
        sethighFilter(20000.f);
    if (highStatus) {
        sethighFilter(highFreq);
    }
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

        subBassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
        bassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
        midsFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
        highFilter.process(juce::dsp::ProcessContextReplacing<float>(block));

}

std::vector<juce::dsp::IIR::Coefficients<float>*> FrequencyCutoffs::getCoefficients() {
    std::vector<juce::dsp::IIR::Coefficients<float>*> tempVec;

    tempVec.push_back(&subBassCoefficient);
    tempVec.push_back(&bassCoefficient);
    tempVec.push_back(&midsCoefficient);
    tempVec.push_back(&highCoefficient);

    return tempVec;
}