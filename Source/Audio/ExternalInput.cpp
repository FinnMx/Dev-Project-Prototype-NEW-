/*
  ==============================================================================

    ExternalInput.cpp
    Created: 14 Mar 2024 2:51:32pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ExternalInput.h"

//==============================================================================
ExternalInput::ExternalInput(juce::AudioDeviceManager* deviceManager) : deviceManager(deviceManager)
{


}

ExternalInput::~ExternalInput()
{
}

void ExternalInput::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {

}

void ExternalInput::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    auto* device = deviceManager->getCurrentAudioDevice();
    auto activeInputChannels = device->getActiveInputChannels();
    auto activeOutputChannels = device->getActiveOutputChannels();
    auto maxInputChannels = activeInputChannels.getHighestBit() + 1;
    auto maxOutputChannels = activeOutputChannels.getHighestBit() + 1;

    for (auto channel = 0; channel < maxOutputChannels; ++channel)
    {
        if ((!activeOutputChannels[channel]) || maxInputChannels == 0)
        {
            bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
        }
        else
        {
            auto actualInputChannel = channel % maxInputChannels;

            if (!activeInputChannels[channel])
            {
                bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
            }
            else
            {
                auto* inBuffer = bufferToFill.buffer->getReadPointer(actualInputChannel,
                    bufferToFill.startSample);
                auto* outBuffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

                for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
                {
                    auto noise = (random.nextFloat() * 2.0f) - 1.0f;
                    outBuffer[sample] = inBuffer[sample] + (inBuffer[sample] * noise * 1.0);
                }
            }
        }
    }
}

void ExternalInput::releaseResources() {

}
