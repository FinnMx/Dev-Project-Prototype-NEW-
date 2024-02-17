#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() :
    cMenuBar(nullptr),
    cGUI(),
    track1LoadButton("Load"), track1PlayButton("Play"), track1StopButton("Stop"),
    track2LoadButton("Load"), track2PlayButton("Play"), track2StopButton("Stop"),
    state(Ready)
{

    GetDesktopResolution(x, y);
    setSize(x, y);

    // Components adding
    addAndMakeVisible(&cGUI);
    addAndMakeVisible(&cMenuBar);
    addAndMakeVisible(&track1LoadButton);
    addAndMakeVisible(&track1PlayButton);
    addAndMakeVisible(&track1StopButton);

    addAndMakeVisible(&track2LoadButton);
    addAndMakeVisible(&track2PlayButton);
    addAndMakeVisible(&track2StopButton);

    audioFormatManager.registerBasicFormats();
    track1LoadButton.onClick = [this] { track1LoadClicked(); };
    track1PlayButton.onClick = [this] { track1PlayClicked(); };
    track1StopButton.onClick = [this] { track1StopClicked(); };
    track1LoadButton.setEnabled(true);
    track1StopButton.setEnabled(false);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
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

void MainComponent::track1LoadClicked() {
    juce::FileChooser chooser("Track 1: Load...", juce::File::getSpecialLocation(juce::File::userMusicDirectory), "*.wav; *.mp3; *.flac;");

    if (chooser.browseForFileToOpen()) {
        // Saves the users choice as a juce::File type, then creates a reader for the filetype.
        juce::File currentFile = chooser.getResult();
        juce::AudioFormatReader* reader = audioFormatManager.createReaderFor(currentFile);

        if(reader != nullptr){
            // Prepares the file to play
            std::unique_ptr<juce::AudioFormatReaderSource> tempSource(new juce::AudioFormatReaderSource(reader, true));

            audioTransportSourceStateChanged(Ready);
            // Gets data from the audio file pointed to from tempSource.
            audioTransportSource.setSource(tempSource.get());

            //prevents the audio from stopping when tempsource is destroyed, practically just passes the pointer to a differnet source
            playsource.reset(tempSource.release());
        }
    }
}

void MainComponent::audioTransportSourceStateChanged(audioTransportSourceStates newState) {
    if (newState != state) {
        state = newState;

        switch (state) {
            case Ready:
                audioTransportSource.stop();
                track1PlayButton.setEnabled(true);
                track1StopButton.setEnabled(false);
                break;
            case Starting:
                track1StopButton.setEnabled(true);
                track1PlayButton.setEnabled(false);
                audioTransportSource.start();
                break;
            case Stopping:
                audioTransportSource.stop();
                audioTransportSource.setPosition(0.0);
                track1PlayButton.setEnabled(true);
                track1StopButton.setEnabled(false);
                break;
        }
    }
}

void MainComponent::track1PlayClicked() {
    audioTransportSourceStateChanged(Starting);
}

void MainComponent::track1StopClicked() {
    audioTransportSourceStateChanged(Stopping);
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::Toolbar::backgroundColourId));

    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::white);
}

void MainComponent::resized()
{
    cGUI.setBounds(getLocalBounds());

    cMenuBar.setBounds(getX(), // X position reletive to main component
        getY(), // Y position reletive to main component
        getWidth(), // Width of component
        getHeight() * 0.025); // Height of component

    track1LoadButton.setBounds(getWidth() * 0.01,
        getHeight() * 0.1,
        getWidth() * 0.03,
        getHeight() * 0.03);

    track1PlayButton.setBounds(getWidth() * 0.01,
        getHeight() * 0.15,
        getWidth() * 0.03,
        getHeight() * 0.03);

    track1StopButton.setBounds(getWidth() * 0.01,
        getHeight() * 0.2,
        getWidth() * 0.03,
        getHeight() * 0.03);
}



//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    audioTransportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    audioTransportSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}
