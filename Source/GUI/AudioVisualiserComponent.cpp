/*
  ==============================================================================

    AudioVisualiserComponent.cpp
    Created: 26 Feb 2024 2:34:09pm
    Author:  New Owner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioVisualiserComponent.h"

//==============================================================================
AudioVisualiserComponent::AudioVisualiserComponent(FrequencyCutoffs* freqCutoffs) : freqCutoffs(freqCutoffs) ,forwardFFT(fftOrder), window(fftSize, juce::dsp::WindowingFunction<float>::hann)
{
    startTimerHz(60);
}

AudioVisualiserComponent::~AudioVisualiserComponent()
{
}

void AudioVisualiserComponent::timerCallback() {
    if (nextFFTBlockReady)
    {
        drawNextFrameOfSpectrum();
        nextFFTBlockReady = false;
        repaint();
    }
}

void AudioVisualiserComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {

}

void AudioVisualiserComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    if (bufferToFill.buffer->getNumChannels() > 0)
    {
        auto* channelData = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples; ++i)
            pushNextSampleIntoFifo(channelData[i]);
    }
}

void AudioVisualiserComponent::pushNextSampleIntoFifo(float sample) noexcept
{
    if (fifoIndex == fftSize)               
    {
        if (!nextFFTBlockReady)            
        {
            juce::zeromem(fftData, sizeof(fftData));
            memcpy(fftData, fifo, sizeof(fifo));
            nextFFTBlockReady = true;
        }

        fifoIndex = 0;
    }

    fifo[fifoIndex++] = sample;            
}

void AudioVisualiserComponent::drawNextFrameOfSpectrum()
{
    // first apply a windowing function to our data
    window.multiplyWithWindowingTable(fftData, fftSize);       // [1]

    // then render our FFT data..
    forwardFFT.performFrequencyOnlyForwardTransform(fftData);  // [2]

    auto mindB = -100.0f;
    auto maxdB = 0.0f;

    for (int i = 0; i < scopeSize; ++i)                         // [3]
    {
        auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - (float)i / (float)scopeSize) * 0.2f);
        auto fftDataIndex = juce::jlimit(0, fftSize / 2, (int)(skewedProportionX * (float)fftSize * 0.5f));
        auto level = juce::jmap(juce::jlimit(mindB, maxdB, juce::Decibels::gainToDecibels(fftData[fftDataIndex])
            - juce::Decibels::gainToDecibels((float)fftSize)),
            mindB, maxdB, 0.0f, 1.0f);

        scopeData[i] = level;                                   // [4]
    }
}


void AudioVisualiserComponent::drawFrame(juce::Graphics& g)
{
    for (int i = 1; i < scopeSize; ++i)
    {
        auto width = getLocalBounds().getWidth();
        auto height = getLocalBounds().getHeight();

        g.drawLine({ (float)juce::jmap(i - 1, 0, scopeSize - 1, 0, width),
                              juce::jmap(scopeData[i - 1], 0.0f, 1.0f, (float)height, 0.0f),
                      (float)juce::jmap(i,     0, scopeSize - 1, 0, width),
                              juce::jmap(scopeData[i],     0.0f, 1.0f, (float)height, 0.0f) });
    }
}


void AudioVisualiserComponent::releaseResources() {

}

void AudioVisualiserComponent::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::PropertyComponent::backgroundColourId));
    g.drawLine(0, getHeight() / 2, getWidth(), getHeight() / 2);
    drawFrame(g);

    /*
    auto bounds = getLocalBounds();
    auto responseArea = bounds;
    auto rW = responseArea.getWidth();
    auto sampleRate = 44100.f;

    std::vector<double> magnitude;
    magnitude.resize(rW);

    for (int i = 0; i < rW; ++i) {
        double mag = 1.f;
        auto freq = juce::mapToLog10((double)i / (double)rW, 20.0, 20000.0);

        auto temp = freqCutoffs->getCoefficients();

        for each (juce::dsp::IIR::Coefficients<float>*cof in temp)
        {
            mag *= cof->getMagnitudeForFrequency(freq, sampleRate);
        }
        magnitude[i] = juce::Decibels::gainToDecibels(mag);
    }

    juce::Path responseCurve;

    const double outputMin = responseArea.getBottom();
    const double outputMax = responseArea.getY();
    auto map = [outputMin, outputMax](double input) {
        return juce::jmap(input, -24.0, 24.0, outputMin, outputMax);
    };

    responseCurve.startNewSubPath(responseArea.getX(), map(magnitude.front()));

    for (size_t i = 1; i < magnitude.size(); ++i) {
        responseCurve.lineTo(responseArea.getX() + i, map(magnitude[i]));
    }
    


    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    */
}

void AudioVisualiserComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
