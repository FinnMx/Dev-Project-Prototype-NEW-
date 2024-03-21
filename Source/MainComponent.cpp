#include "MainComponent.h"
#include "wtypes.h"
#include <iostream>
using namespace std;

//==============================================================================
MainComponent::MainComponent() : juce::AudioAppComponent(deviceManager),
    menuBar(this)
{
    getMidiDevice();
    deviceManager.initialise(2, 2, nullptr, true);
    audioSettings.reset(new juce::AudioDeviceSelectorComponent(deviceManager, 0, 2, 0, 2, true, true, true,true));
    audioSettingsWindow.setAudioSettings(audioSettings.get());

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
    addAndMakeVisible(dubSiren);

    audioWindow = new PopoutWindow("Audio Settings", &audioSettingsWindow, x, y);
    midiHandler.readSettingsFile();
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
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    horizontal = desktop.right;
    vertical = desktop.bottom;
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    externalInput.prepareToPlay(samplesPerBlockExpected, sampleRate);
    track1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    track2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    freqCutoffs.prepareToPlay(samplesPerBlockExpected, sampleRate);
    circularBuffer.prepareToPlay(samplesPerBlockExpected, 44100.0);
    tenBandEQ.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&externalInput,false);
    mixerSource.addInputSource(&track1, false);
    mixerSource.addInputSource(&track2, false);

    dubSirenPlayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    mixerSource.getNextAudioBlock(bufferToFill);
    tenBandEQ.getNextAudioBlock(bufferToFill);
    freqCutoffs.getNextAudioBlock(bufferToFill);
    dubSirenPlayer.getNextAudioBlock(bufferToFill);

    circularBuffer.getNextAudioBlock(bufferToFill);
    delayComponent.getNextAudioBlock(bufferToFill);
    killEQComponent.getNextAudioBlock(bufferToFill);

    rmsMasterLeft = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(0, 0, bufferToFill.buffer->getNumSamples()));
    rmsMasterRight = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(1, 0, bufferToFill.buffer->getNumSamples()));

}

void MainComponent::releaseResources() {
    externalInput.releaseResources();
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
    track1.releaseResources();
    track2.releaseResources();
    circularBuffer.releaseResources();

    dubSirenPlayer.releaseResources();
}
//=============================================================================

juce::PopupMenu MainComponent::getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName) {
    juce::PopupMenu menu;

    if (topLevelMenuIndex == 0)
    {
        menu.addItem("Audio Settings", [&]() {
            audioWindow->setCentrePosition(getWidth() * 0.5, getHeight() * 0.5);
            audioWindow->setVisible(true);
            });
        menu.addItem("Key Bindings", [&]() {
            keyBindingsWindow.setCentrePosition(getWidth() * 0.5, getHeight() * 0.5);
            //keyBindingsWindow.setVisible(!keyBindingsWindow.isShowing());
            if (!keyBindingsWindow.isShowing())
                addAndMakeVisible(keyBindingsWindow);
            else{
                removeChildComponent(&keyBindingsWindow);
            }
            });
    }
    return menu;
}

juce::StringArray MainComponent::getMenuBarNames() {
    return { "Settings", "Help" };
}

void MainComponent::menuItemSelected(int menuItemID, int topLevelMenuIndex) {
}
//==============================================================================
void MainComponent::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) {
    int value;
    int input = processMidiInput(message, value);
        switch (midiHandler.returnCorrespondingComponent(input))
        {
        case 1: // INPUT A
            inputAComponent.handleMidi(midiHandler.returnCorrespondingAction(input), value);
            break;
        case 2: // INPUT B
            inputBComponent.handleMidi(midiHandler.returnCorrespondingAction(input), value);
            break;
        case 3: // THUMBNAIL VIEW
            break;
        case 4: // 10BAND EQ
            break;
        case 5: // DUB SIREN
            dubSiren.handleMidi(midiHandler.returnCorrespondingAction(input), value);
            break;
        case 6: // REVERB
            reverbComponent.handleMidi(midiHandler.returnCorrespondingAction(input), value);
            break;
        case 7: // DELAY
            delayComponent.handleMidi(midiHandler.returnCorrespondingAction(input), value);
            break;
        case 8: // KILL EQ
            killEQComponent.handleMidi(midiHandler.returnCorrespondingAction(input));
            break;
        }

        //RE ENABLE THIS SO BINDS CAN BE MADE
    makeBind(input);
}

void MainComponent::makeBind(int input) {
    if (keyBindingsWindow.isWaitingForBind()) {
        std::pair<int, int> componentAndAction = keyBindingsWindow.getComponentAndAction();
        midiHandler.bindKey(input, componentAndAction.first, componentAndAction.second);
        keyBindingsWindow.resetBindWait();
    }
}

int MainComponent::processMidiInput(juce::MidiMessage message, int& value) {
    value = 0;
    if (message.isNoteOnOrOff()) { return message.getNoteNumber() * message.getVelocity() + message.getChannel(); }
    if (message.isController()) { value = message.getControllerValue(); return message.getControllerNumber() * 10 + message.getChannel(); }
    return NULL;
}

unsigned int MainComponent::hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

void MainComponent::setMidiInput(juce::MidiDeviceInfo& id) {
    deviceManager.setMidiInputDeviceEnabled(id.identifier, true);
    deviceManager.addMidiInputDeviceCallback(id.identifier, this);
}

void MainComponent::getMidiDevice() {
    auto midiInputs = juce::MidiInput::getAvailableDevices();

    juce::StringArray midiInputNames;

    for (auto input : midiInputs)
        midiInputNames.add(input.name);

    // find the first enabled device and use that by default
    for (auto input : midiInputs)
    {
        if (deviceManager.isMidiInputDeviceEnabled(input.identifier))
        {
            setMidiInput(input);
            break;
        }
    }

    if (!midiInputs.isEmpty())
        setMidiInput(midiInputs.getFirst());
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

    dubSiren.setBounds(
        getX(),
        getHeight() * 0.39,
        getWidth() * 0.15,
        getHeight() * 0.32
    );

    reverbComponent.setBounds(
        getWidth() * 0.15,
        getHeight() * 0.39,
        getWidth() * 0.85,
        getHeight() * 0.16
    );

    delayComponent.setBounds(
        getWidth() * 0.15,
        getHeight() * 0.55,
        getWidth() * 0.85,
        getHeight() * 0.16
    );


    //-----------------------------------------------
    //Bottom Rack

    killEQComponent.setBounds(
        getX(),
        getHeight() * 0.72,
        getWidth(),
        getHeight() * 0.275

    );

    keyBindingsWindow.setBounds(getX(), getHeight(), getWidth(), getHeight() * 0.955);

    //-----------------------------------------------
    //Settings Menus
    //audioSettingsWindow.centreWithSize(getWidth() * 0.4, getHeight() * 0.5);
}
