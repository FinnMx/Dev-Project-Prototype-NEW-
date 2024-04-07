/*
  ==============================================================================

    FrequencyLevelThread.h
    Created: 6 Apr 2024 9:56:06pm
    Author:  New Owner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FrequencyLevelThread  : public juce::Thread,
                              public juce::Timer
{
public:
    FrequencyLevelThread(float* frequencies);
    ~FrequencyLevelThread() override;

    void run() override;
    void timerCallback() override;

    void updateBuffer(juce::AudioBuffer<float> newBuffer);

    class LevelCalculatorThread : public juce::Thread {
    public:
        LevelCalculatorThread() : juce::Thread("Level Calculator Thread") {

        }
        ~LevelCalculatorThread() {
        }

        void setParameters(float* averageLevel, juce::AudioBuffer<float>* newBuffer, float newFrequency) {
            avgLevel = averageLevel;
            buffer = newBuffer;
            frequency = newFrequency;
        }

        void run() {
            auto tempBuffer = *buffer; // Input audio buffer
            const int numSamples = tempBuffer.getNumSamples();
            const int numChannels = tempBuffer.getNumChannels();

            // Define the frequency of interest
            const float frequencyOfInterest = frequency; // Example: 1000 Hz

            // Calculate the number of samples in one cycle of the frequency of interest
            const float sampleRate = 44100.f;
            const float samplesPerCycle = sampleRate / frequencyOfInterest;

            // Define the window size around the frequency of interest
            const int windowSize = static_cast<int>(samplesPerCycle * 2); // Example: Two cycles around the frequency

            // Initialize RMS accumulator
            float rmsAccumulator = 0.0f;

            // Iterate over each channel
            for (int channel = 0; channel < numChannels; ++channel)
            {
                // Iterate over the audio samples
                for (int i = 0; i < numSamples - windowSize; ++i)
                {
                    // Calculate RMS within the window around the frequency of interest
                    float sumSquared = 0.0f;
                    for (int j = i; j < i + windowSize; ++j)
                    {
                        float sample = tempBuffer.getSample(channel, j);
                        sumSquared += sample * sample;
                    }

                    float rms = std::sqrt(sumSquared / windowSize);
                    rmsAccumulator += rms;
                }
            }

            // Calculate the average RMS across channels and samples
            *avgLevel = rmsAccumulator / (numChannels * (numSamples - windowSize));
            signalThreadShouldExit();
        }

    private:
        float* avgLevel;
        float frequency;
        juce::AudioBuffer<float>* buffer;
    };

private:
    juce::AudioBuffer<float> buffer;

    float* frequencies;
    int elapsedTime{ 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencyLevelThread)
};
