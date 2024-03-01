/*
  ==============================================================================

    CircularBuffer.h
    Created: 22 Feb 2024 11:01:45pm
    Author:  Finn

  ==============================================================================
*/

#pragma once
#include <TrackAudioPlayer.h>
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
    /*
    void fillBuffer(int channel, int bufferSize, int delayBufferSize, float* channelData);
    void readFromBuffer(int channel, int bufferSize, int delayBufferSize, const juce::AudioSourceChannelInfo& bufferToFill, juce::AudioBuffer<float>& delayBuffer);
    */

    bool delayStatus{ false };
    float delayFeedback{ 0.f };
    float delayTime{ 0.f };
    float rampingVal{ 0.f };
    float frequencyBand{ 14000.f };

    int totalNumInputChannels{ 2 }, totalNumOutputChannels{ 2 };

    //========================================================================================
    static constexpr auto effectDelaySamples = 88200;
    juce::dsp::DelayLine<float> delay{ effectDelaySamples };
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Thiran> linear{ effectDelaySamples };

    //juce::IIRFilter filter;

    //====================================================================
    // TEST STUFF
    juce::AudioBuffer<float> copyBuffer;

    juce::dsp::IIR::Coefficients<float> filterCoefficient;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> bandFilter;
    //====================================================================
    
    //SMOOTHER FOR THE DELAY TIME
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoother{ 0.f };  
    juce::dsp::WindowingFunction<float> windower{ 480, juce::dsp::WindowingFunction<float>::WindowingMethod::hamming };

    float yn{ 0 };


    std::array<float, 2> delayValue{ {} };
    std::array<float, 2> lastDelayOutput;
    std::array<juce::LinearSmoothedValue<float>, 2> delayFeedbackVolume;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CircularBuffer)
};
