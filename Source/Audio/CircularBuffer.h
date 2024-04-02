/*
  ==============================================================================

    CircularBuffer.h
    Created: 22 Feb 2024 11:01:45pm
    Author:  Finn

  ==============================================================================
*/

#pragma once
#include <Audio/TrackAudioPlayer.h>
#include <JuceHeader.h>

//==============================================================================
/*
*/
class CircularBuffer : public juce::AudioSource
{
public:
    CircularBuffer();
    ~CircularBuffer() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    std::vector<juce::dsp::IIR::Coefficients<float>*> getCoefficients();

    void setDelayFeedback(float newFeedback);
    void setDelayStatus(bool newStatus);
    void setDelayTime(float newTime);
    void setDelayCutoffFrequency(float newFrequencyCutoff);

private:
    bool delayStatus{ false };
    float delayFeedback{ 0.f };
    float delayTime{ 0.f };
    float rampingVal{ 0.f };

    float lowFrequencyBand{ 20000.f };
    float highFrequencyBand{ 18500.f };

    int totalNumInputChannels{ 2 }, totalNumOutputChannels{ 2 };

    //========================================================================================
    static constexpr auto effectDelaySamples = 88200;

    juce::dsp::DelayLine<float> delay{ effectDelaySamples };
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Lagrange3rd> linear{ effectDelaySamples };

    //juce::IIRFilter filter;

    //====================================================================
    // TEST STUFF
    juce::AudioBuffer<float> copyBuffer;

    juce::dsp::IIR::Coefficients<float> lowFilterCoefficient;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowFilter;
    juce::dsp::IIR::Coefficients<float> highFilterCoefficient;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highFilter;
    //MORE TEST STUFF
    juce::IIRFilter testFilterLowL;
    juce::IIRFilter testFilterLowR;
    juce::IIRFilter testFilterHighL;
    juce::IIRFilter testFilterHighR;

    //====================================================================
    
    //SMOOTHER FOR THE DELAY TIME
    //juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear>* smoother;  
    juce::dsp::WindowingFunction<float> windower{ 480, juce::dsp::WindowingFunction<float>::WindowingMethod::hamming };

    float coef = 1.0f - std::exp(-1.0f / 0.05f);

    float yn{ 0 };


    std::array<float, 2> delayValue{ {} };
    std::array<float, 2> lastDelayOutput;
    std::array<juce::LinearSmoothedValue<float>, 2> delayFeedbackVolume;
    std::array<juce::LinearSmoothedValue<float>, 2> smoother;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CircularBuffer)
};
