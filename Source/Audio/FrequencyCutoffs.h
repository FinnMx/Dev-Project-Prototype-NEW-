/*
  ==============================================================================

    FrequencyCutoffs.h
    Created: 27 Feb 2024 1:16:27pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FrequencyCutoffs : public juce::AudioSource
{
public:
    FrequencyCutoffs();
    ~FrequencyCutoffs() override;

    void setSubBassFilter(float newFreq);
    void setbassFilterr(float newFreq);
    void setmidsFilter(float newFreq);
    void sethighFilter(float newFreq);

    void setNewSubBassFreq(float newFreq);
    void setNewBassFreq(float newFreq);
    void setNewMidsFreq(float newFreq);
    void setNewHighFreq(float newFreq);

    void setSubBassStatus(bool newsubBassStatus);
    void setBassStatus(bool newbassStatus);
    void setMidsStatus(bool newmidsStatus);
    void setHighStatus(bool newhighStatus);

    std::vector<juce::dsp::IIR::Coefficients<float>*> getCoefficients();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

private:
    int totalNumInputChannels{ 2 }, totalNumOutputChannels{ 2 };

    bool subBassStatus{ false };
    bool bassStatus{ false };
    bool midsStatus{ false };
    bool highStatus{ false };

    float subBassFreq{ 95.f };
    float bassFreq{ 250.f };
    float midsFreq{ 3500.f };
    float highFreq{ 5000.f };

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> subBassSmoother{ 0.f };
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> bassSmoother{ 0.f };
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> midsSmoother{ 0.f };
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> highSmoother{ 0.f };

    juce::dsp::IIR::Coefficients<float> subBassCoefficient;
    juce::dsp::IIR::Coefficients<float> bassCoefficient;
    juce::dsp::IIR::Coefficients<float> midsCoefficient;
    juce::dsp::IIR::Coefficients<float> highCoefficient;

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> subBassFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> bassFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> midsFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highFilter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequencyCutoffs)
};