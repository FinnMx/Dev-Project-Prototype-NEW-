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
    subBassCoefficient = *juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, newFreq, 1);
    *subBassFilter.state = subBassCoefficient;
    subBassFreq = newFreq;
}

void FrequencyCutoffs::setbassFilterr(float newFreq) {
    bassCoefficient = *juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, newFreq, 0.7);
    *bassFilter.state = bassCoefficient;
    bassFreq = newFreq;
}

void FrequencyCutoffs::setmidsFilter(float newFreq) {
    midsCoefficient = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, newFreq, 1);
    *midsFilter.state = midsCoefficient;
    midsFreq = newFreq;
}

void FrequencyCutoffs::sethighFilter(float newFreq) {
    highCoefficient = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, newFreq, 1);
    *highFilter.state = highCoefficient;
    highFreq = newFreq;
}

void FrequencyCutoffs::setSubBassStatus(bool newsubBassStatus) {
    if(!newsubBassStatus)
        subBassSmoother.setTargetValue(20.f);
    if(newsubBassStatus){
        subBassStatus = newsubBassStatus;
        subBassSmoother.setTargetValue(subBassFreq);
    }
}
void FrequencyCutoffs::setBassStatus(bool newbassStatus) {
    if (!newbassStatus)
        bassSmoother.setTargetValue(20.f);
    if (newbassStatus) {
        bassStatus = newbassStatus;
        bassSmoother.setTargetValue(bassFreq);
    }
}
void FrequencyCutoffs::setMidsStatus(bool newmidsStatus) {
    if (!newmidsStatus)
        midsSmoother.setTargetValue(20000.f);
    if(newmidsStatus){
    midsStatus = newmidsStatus;
    midsSmoother.setTargetValue(midsFreq);
    }
}
void FrequencyCutoffs::setHighStatus(bool newhighStatus) {
    if (!newhighStatus)
        highSmoother.setTargetValue(20000.f);
    if(highStatus){
        highStatus = newhighStatus;
        highSmoother.setTargetValue(highFreq);
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

    switch (subBassStatus) {
        case true:
            setSubBassFilter(subBassSmoother.getNextValue());
            subBassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
            break;
    }
    switch (bassStatus) {
    case true:
        setbassFilterr(bassSmoother.getNextValue());
        bassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
        break;
    }
    switch (midsStatus) {
    case true:
        setmidsFilter(midsSmoother.getNextValue());
        midsFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
        break;
    }
    switch (highStatus) {
    case true:
        sethighFilter(highSmoother.getNextValue());
        highFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
        break;
    }

}

std::vector<juce::dsp::IIR::Coefficients<float>*> FrequencyCutoffs::getCoefficients(){
    std::vector<juce::dsp::IIR::Coefficients<float>*> tempVec;

    tempVec.push_back(&subBassCoefficient);
    tempVec.push_back(&bassCoefficient);
    tempVec.push_back(&midsCoefficient);
    tempVec.push_back(&highCoefficient);

    return tempVec;
}