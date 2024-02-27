/*
  ==============================================================================

    Crossfade.h
    Created: 27 Feb 2024 3:46:02pm
    Author:  Finn

  ==============================================================================
*/
#include <JuceHeader.h>

class Crossfade
{
public:
    enum ActiveBuffer { leftBuffer, rightBuffer };

    Crossfade() = default;

    /**
        Resets the crossfade, setting the sample rate and ramp length.

        @param sampleRate           The current sample rate.
        @param rampLengthInSeconds  The duration of the ramp in seconds.
    */
    void reset(double sampleRate, double rampLengthInSeconds)
    {
        smoothedGain.reset(sampleRate, rampLengthInSeconds);
    }

    /**
        Sets the active buffer. I.e. which one should be written to the output.

        @param buffer   An enum value indicating which buffer to output.
    */
    void setActiveBuffer(ActiveBuffer buffer)
    {
        if (buffer == leftBuffer)
            setGain(1.0);
        else
            setGain(0.0);
    }

    /**
        Can be used to set a custom gain level to combine the two buffers.

        @param gain     The gain level of the left buffer.
    */
    void setGain(double gain)
    {
        smoothedGain.setTargetValue(std::clamp(gain, 0.0, 1.0));
    }

    /**
        Applies the crossfade.

        Output buffer can be the same buffer as either of the inputs.

        All buffers should have the same number of channels and samples as each
        other, but if not, then the minimum number of channels/samples will be
        used.

        @param leftBuffer   The left input buffer to read from.
        @param rightBuffer  The right input buffer to read from.
        @param outputBuffer The buffer in which to store the result of the crossfade.
    */
    template<typename LeftFloatType, typename RightFloatType, typename OutFloatType>
    void process(const juce::AudioBuffer<LeftFloatType>& leftBuffer,
        const juce::AudioBuffer<RightFloatType>& rightBuffer,
        juce::AudioBuffer<OutFloatType>& outputBuffer)
    {
        // find the lowest number of channels available across all buffers
        const auto channels = std::min({ leftBuffer.getNumChannels(),
                                          rightBuffer.getNumChannels(),
                                          outputBuffer.getNumChannels() });
        // find the lowest number of samples available across all buffers
        const auto samples = std::min({ leftBuffer.getNumSamples(),
                                         rightBuffer.getNumSamples(),
                                         outputBuffer.getNumSamples() });

        for (int channel = 0; channel < channels; ++channel)
        {
            for (int sample = 0; sample < samples; ++sample)
            {
                // obtain the input samples from their respective buffers
                const auto left = leftBuffer.getSample(channel, sample);
                const auto right = rightBuffer.getSample(channel, sample);

                // get the next gain value in the smoothed ramp towards target
                const auto gain = smoothedGain.getNextValue();

                // calculate the output sample as a mix of left and right
                auto output = left * gain + right * (1.0 - gain);

                // store the output sample value
                outputBuffer.setSample(channel, sample, static_cast<OutFloatType> (output));
            }
        }
    }

private:
    juce::SmoothedValue<double, juce::ValueSmoothingTypes::Linear> smoothedGain;
};