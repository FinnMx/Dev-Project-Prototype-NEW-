#pragma once
#include "wtypes.h"
#include <iostream>

#include <JuceHeader.h>
#include <CircularBuffer.h>
#include <TrackAudioPlayer.h>
#include <GUI/DelayComponent.h>
#include <GUI/InputComponent.h>
#include <GUI/KillEQComponent.h>
#include <GUI/MasterComponent.h>
#include <GUI/ReverbComponent.h>
#include <GUI/TenBandComponent.h>
#include <GUI/TrackThumbnailComponent.h>
#include <GUI/MeterComponent.h>
#include <FrequencyCutoffs.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::MidiInputCallback
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
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    void setMidiInput(juce::MidiDeviceInfo& id);
    void getMidiDevice();

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    void GetDesktopResolution(int& horizontal, int& vertical);

private:
    //==============================================================================
    // Your private member variables go here...
    int x, y;
    float rmsMasterLeft, rmsMasterRight;

    //look and feel

    //MIDI
    juce::AudioDeviceManager deviceManager;

    // Child components
    juce::MenuBarComponent menuBar;

    // Audio components
    juce::MixerAudioSource mixerSource;
    //juce::MixerAudioSource effectsMixer;

    juce::AudioFormatManager formatManager;
    TrackAudioPlayer track1{formatManager};
    TrackAudioPlayer track2{formatManager};
    CircularBuffer circularBuffer{};
    FrequencyCutoffs freqCutoffs;

    // Thumbnail component
    juce::AudioThumbnailCache cache{ 100 };
    TrackThumbnailComponent trackAThumbnailComponent{ "Track A", cache, formatManager, &track1 };
    TrackThumbnailComponent trackBThumbnailComponent{ "Track B", cache, formatManager, &track2 };

    InputComponent inputAComponent{"Track A",&track1, formatManager,&trackAThumbnailComponent};
    InputComponent inputBComponent{"Track B",&track2, formatManager,&trackBThumbnailComponent };

    MasterComponent masterComponent{ &rmsMasterLeft, &rmsMasterRight };

    ReverbComponent reverbComponent{&track1, &track2};
    DelayComponent delayComponent{&circularBuffer};
    KillEQComponent killEQComponent{ &freqCutoffs };

    TenBandComponent tenBandComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
