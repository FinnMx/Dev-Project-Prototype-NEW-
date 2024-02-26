/*
  ==============================================================================

    AudioVisualiserComponent.h
    Created: 26 Feb 2024 2:34:09pm
    Author:  New Owner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AudioVisualiserComponent : public juce::AudioAppComponent,
                                 public juce::Timer
{
public:
    AudioVisualiserComponent();
    ~AudioVisualiserComponent() override;

    void timerCallback() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (juce::Graphics&) override;
    void drawFrame(juce::Graphics& g);
    void resized() override;

    void pushNextSampleIntoFifo(float sample) noexcept;
    void drawNextFrameOfSpectrum();

    enum
    {
        fftOrder = 11,
        fftSize = 1 << fftOrder,
        scopeSize = 512
    };

private:

    juce::dsp::FFT forwardFFT;                      
    juce::dsp::WindowingFunction<float> window;     

    float fifo[fftSize];                           
    float fftData[2 * fftSize];                    
    int fifoIndex = 0;                             
    bool nextFFTBlockReady = false;                
    float scopeData[scopeSize];                    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioVisualiserComponent)
};
