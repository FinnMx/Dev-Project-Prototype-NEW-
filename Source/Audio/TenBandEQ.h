/*
  ==============================================================================

    TenBandEQ.h
    Created: 6 Mar 2024 3:47:02pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <Threads/FrequencyLevelThread.h>

//==============================================================================
/*
*/
class TenBandEQ  : public juce::AudioSource
{
public:
    TenBandEQ();
    ~TenBandEQ() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void processFrequencies();

    void setNewGain(int sliderNum, float newGain);

private:
    void calculateFrequencyBandRMS();

    int totalNumInputChannels{ 2 }, totalNumOutputChannels{ 2 };
    float frequencies[10] = { 30.f, 62.f, 125.f, 250.f, 500.f, 1000.f, 2000.f, 4000.f, 8000.f, 16000.f };

    std::vector<juce::dsp::IIR::Coefficients<float>> coefficients;
    std::vector<juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>*> filters;

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> freq30;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> freq62;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> freq125;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> freq250;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> freq500;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> freq1000;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> freq2000;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> freq4000;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> freq8000;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> freq16000;

    FrequencyLevelThread frequencyLevelThread{ frequencies };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TenBandEQ)
};
