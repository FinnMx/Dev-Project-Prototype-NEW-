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
    auto& buffer = *(bufferToFill.buffer);
    const auto* const* read = buffer.getArrayOfReadPointers();
    auto* const* write = buffer.getArrayOfWritePointers();
    for (auto channel = 0; channel < buffer.getNumChannels(); ++channel) {
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample) {
            write[channel][sample] = read[channel][sample];
        }
    }
}

void ExternalInput::releaseResources() {

}
