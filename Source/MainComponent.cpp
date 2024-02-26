#include "MainComponent.h"
#include "wtypes.h"
#include <iostream>
using namespace std;

//==============================================================================
MainComponent::MainComponent() :
    menuBar(nullptr)
{
    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(2, 2);
    }

    GetDesktopResolution(x, y);
    setSize(x, y);

    formatManager.registerBasicFormats();

    // Components adding
    addAndMakeVisible(inputAComponent);
    addAndMakeVisible(inputBComponent);
    addAndMakeVisible(menuBar);
    addAndMakeVisible(masterComponent);
    addAndMakeVisible(trackAThumbnailComponent);
    addAndMakeVisible(trackBThumbnailComponent);
    addAndMakeVisible(tenBandComponent);
    addAndMakeVisible(reverbComponent);
    addAndMakeVisible(delayComponent);
    addAndMakeVisible(killEQComponent);

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

// Get the horizontal and vertical screen sizes in pixel
void MainComponent::GetDesktopResolution(int& horizontal, int& vertical)
{
    RECT desktop;
    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    // The top left corner will have coordinates (0,0)
    // and the bottom right corner will have coordinates
    // (horizontal, vertical)
    horizontal = desktop.right;
    vertical = desktop.bottom;
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    track1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    track2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    circularBuffer.prepareToPlay(samplesPerBlockExpected, 44100.0);

    mixerSource.addInputSource(&track1, false);
    mixerSource.addInputSource(&track2, false);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    //bufferToFill.clearActiveBufferRegion();
    mixerSource.getNextAudioBlock(bufferToFill);
    circularBuffer.getNextAudioBlock(bufferToFill);
    killEQComponent.getNextAudioBlock(bufferToFill);

    rmsMasterLeft = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(0, 0, bufferToFill.buffer->getNumSamples()));
    rmsMasterRight = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(1, 0, bufferToFill.buffer->getNumSamples()));
}

void MainComponent::releaseResources() {
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
    track1.releaseResources();
    track2.releaseResources();
    circularBuffer.releaseResources();
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::PropertyComponent::backgroundColourId));

    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::white);
}

void MainComponent::resized()
{

    menuBar.setBounds(getX(), // X position reletive to main component
        getY(), // Y position reletive to main component
        getWidth(), // Width of component
        getHeight() * 0.025); // Height of component

    //-----------------------------------------------
    //Top Rack

    inputAComponent.setBounds(
        getX(),
        getHeight() * 0.035,
        getWidth() * 0.125,
        getHeight() * 0.35
    );

    inputBComponent.setBounds(
        getWidth() * 0.125,
        getHeight() * 0.035,
        getWidth() * 0.125,
        getHeight() * 0.35
    );

    masterComponent.setBounds(
        getWidth() * 0.25,
        getHeight() * 0.035,
        getWidth() * 0.15,
        getHeight() * 0.35
    );

    trackAThumbnailComponent.setBounds(
        getWidth() * 0.4,
        getHeight() * 0.035,
        getWidth() * 0.375,
        getHeight() * 0.175
    );

    trackBThumbnailComponent.setBounds(
        getWidth() * 0.4,
        getHeight() * 0.21,
        getWidth() * 0.375,
        getHeight() * 0.175
    );

    tenBandComponent.setBounds(
        getWidth() * 0.775,
        getHeight() * 0.035,
        getWidth() * 0.225,
        getHeight() * 0.35
    );

    //-----------------------------------------------
    //Middle Rack's

    reverbComponent.setBounds(
        getX(),
        getHeight() * 0.39,
        getWidth(),
        getHeight() * 0.125
    );

    delayComponent.setBounds(
        getX(),
        getHeight() * 0.52,
        getWidth(),
        getHeight() * 0.125
    );


    //-----------------------------------------------
    //Bottom Rack

    killEQComponent.setBounds(
        getX(),
        getHeight() * 0.65,
        getWidth(),
        getHeight() * 0.315
    );
}
