#pragma once
#include "wtypes.h"
#include <iostream>

#include <JuceHeader.h>
#include <GUIComponent.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    void GetDesktopResolution(int& horizontal, int& vertical);

private:
    //==============================================================================
    // Your private member variables go here...
    int x, y;

    // Enum States
    enum audioTransportSourceStates {
        Ready,
        Starting,
        Stopping
    };

    audioTransportSourceStates state;

    // Child components
    juce::MenuBarComponent cMenuBar;
    GUIComponent cGUI;

    //Audio related components
    juce::AudioFormatManager audioFormatManager;
    juce::AudioTransportSource audioTransportSource;

    std::unique_ptr<juce::AudioFormatReaderSource> playsource;

    //functions
    void track1LoadClicked();
    void track1PlayClicked();
    void track1StopClicked();
    void audioTransportSourceStateChanged(audioTransportSourceStates newState);

    //Buttons
    juce::TextButton track1LoadButton;
    juce::TextButton track1PlayButton;
    juce::TextButton track1StopButton;

    juce::TextButton track2LoadButton;
    juce::TextButton track2PlayButton;
    juce::TextButton track2StopButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
