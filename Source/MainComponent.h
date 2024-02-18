#pragma once
#include "wtypes.h"
#include <iostream>

#include <JuceHeader.h>
#include <TrackAudioPlayer.h>
#include <GUI/DelayComponent.h>
#include <GUI/FileBrowserComponent.h>
#include <GUI/InputComponent.h>
#include <GUI/KillEQComponent.h>
#include <GUI/MasterComponent.h>
#include <GUI/ReverbComponent.h>
#include <GUI/TenBandComponent.h>
#include <GUI/TrackThumbnailComponent.h>

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

    //look and feel

    // Child components
    juce::MenuBarComponent menuBar;

    // Audio components
    juce::MixerAudioSource mixerSource;
    juce::AudioFormatManager formatManager;
    TrackAudioPlayer track1{formatManager};
    TrackAudioPlayer track2{formatManager};

    DelayComponent delayComponent;

    InputComponent inputAComponent{"Track A",&track1, formatManager};
    InputComponent inputBComponent{"Track B",&track1, formatManager};

    KillEQComponent killEQComponent;
    MasterComponent masterComponent;
    ReverbComponent reverbComponent;
    TrackThumbnailComponent trackThumbnailComponent;
    TenBandComponent tenBandComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
