#pragma once
#include "wtypes.h"
#include <iostream>

#include <JuceHeader.h>
#include <Audio/CircularBuffer.h>
#include <Audio/TrackAudioPlayer.h>
#include <Audio/FrequencyCutoffs.h>
#include <Audio/TenBandEQ.h>

#include <GUI/DelayComponent.h>
#include <GUI/InputComponent.h>
#include <GUI/KillEQComponent.h>
#include <GUI/MasterComponent.h>
#include <GUI/ReverbComponent.h>
#include <GUI/TenBandComponent.h>
#include <GUI/TrackThumbnailComponent.h>
#include <GUI/MeterComponent.h>
#include <GUI/DubSirenComponent.h>

#include <Settings/AudioSettingsComponent.h>
#include <Settings/keyBindingsComponent.h>
#include <Settings/PopoutWindow.h>


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::MidiInputCallback,
                       public juce::MenuBarModel
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
    juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName) override;
    juce::StringArray getMenuBarNames() override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

    //==============================================================================
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    void setMidiInput(juce::MidiDeviceInfo& id);
    void getMidiDevice();
    void setMidiSet();

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    void GetDesktopResolution(int& horizontal, int& vertical);

private:
    //==============================================================================
    int x, y;
    float rmsMasterLeft, rmsMasterRight;

    juce::AudioDeviceManager deviceManager;
    std::unique_ptr<juce::AudioDeviceSelectorComponent> audioSettings;

    //Setting windows
    juce::ScopedPointer<PopoutWindow> audioWindow;
    AudioSettingsComponent audioSettingsWindow;
    juce::ScopedPointer<PopoutWindow> keyBindWindow;
    KeyBindingsComponent keyBindingsWindow;

    //MIDI
    int midiset{ 1 }; // make this an ENUM

    // Child components
    juce::MenuBarComponent menuBar;

    // Audio components
    juce::MixerAudioSource mixerSource;

    juce::AudioFormatManager formatManager;
    TrackAudioPlayer track1{formatManager};
    TrackAudioPlayer track2{formatManager};
    CircularBuffer circularBuffer{};
    FrequencyCutoffs freqCutoffs;
    TenBandEQ tenBandEQ;

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

    DubSiren dubSirenPlayer;
    DubSirenComponent dubSiren{ &dubSirenPlayer };

    TenBandComponent tenBandComponent{ &tenBandEQ };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
