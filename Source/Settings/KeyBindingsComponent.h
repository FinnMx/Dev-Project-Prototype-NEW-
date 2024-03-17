/*
  ==============================================================================

    KeyBindingsComponent.h
    Created: 4 Mar 2024 4:11:59pm
    Author:  New Owner

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <algorithm>

//==============================================================================
/*
*/
class KeyBindingsComponent  : public juce::Component,
                              public::juce::Button::Listener
{
public:
    KeyBindingsComponent();
    ~KeyBindingsComponent() override;

    void buttonClicked(juce::Button* button) override;

    bool isWaitingForBind();
    std::pair<int, int> getComponentAndAction();
    void resetBindWait();

    void setComponentAndAction(int component, juce::Button* button);

    void refreshPage();

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextButton pageForward{ " > " }, pageBackwards{ " < " };
    int currentTab{ 0 };
    bool waitingForBind{ false };

    std::pair<int, int> currentComponentAndAction{ std::make_pair(NULL, NULL) };
    int getActionIndex(juce::Button* button);
    int findIndex(juce::TextButton arr[], juce::Button* button);

    std::string headings[8] = { "Input A Bindings", "Input B Bindings", "Thumbnail Bindings", "10 Band Bindings","Dub Siren Bindings", "Reverb Bindings", "Delay Bindings", "Kill EQ Bindings" };
    std::string currentHeading;

    juce::TextButton inputAParameters[5];
    juce::TextButton inputBParameters[5];
    juce::TextButton thumbnailParameters[2];
    juce::TextButton tenBandParameters[10];
    juce::TextButton dubSirenParameters[3];
    juce::TextButton reverbParameters[3];
    juce::TextButton delayParameters[4];
    juce::TextButton killEQParameters[4];
    int parameterSizes[8]{ 5, 5, 2, 10, 3, 3, 4, 4 };

    juce::TextButton* parameters[8];

    //float parameterY[10]{ 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55,0.6, 0.65 };

    float inputAButtonBounds[5][4] = { {0.003, 0.075,0.02,0.04} , {0.003,0.12,0.02,0.04}, {0.003,0.165,0.02,0.04}, {0.003,0.21,0.02,0.04}, {0.078,0.075,0.02,0.2} };
    float inputBButtonBounds[5][4] = { {0.128,0.075,0.02,0.04} ,{0.128,0.12,0.02,0.04},{0.128,0.165,0.02,0.04},{0.128,0.21,0.02,0.04},{0.203,0.075,0.02,0.2} };
    float thumbnailButtonBounds[2][4] = { {0.,0.,0.,0.} ,{0.,0.,0.,0.}};

    float tenBandButtonBounds[10][4] = { {0.778,0.085,0.02,0.2} ,{0.8,0.085,0.02,0.2},{0.822,0.085,0.02,0.2},{0.844,0.085,0.02,0.2},{0.866,0.085,0.02,0.2},
                                         {0.888,0.085,0.02,0.2} ,{0.91,0.085,0.02,0.2},{0.932,0.085,0.02,0.2},{0.954,0.085,0.02,0.2},{0.976,0.085,0.02,0.2} };

    float dubSirenButtonBounds[3][4] = { {0.012,0.495,0.05,0.075} ,{0.5,0.5,0.5,0.5},{0.5,0.5,0.5,0.5} };
    float reverbButtonBounds[3][4] = { {0.6,0.6,0.6,0.6} ,{0.6,0.6,0.6,0.6},{0.6,0.6,0.6,0.6} };
    float delayButtonBounds[4][4] = { {0.7,0.7,0.7,0.7} ,{0.7,0.7,0.7,0.7},{0.7,0.7,0.7,0.7},{0.7,0.7,0.7,0.7} };
    float killEQButtonBounds[4][4] = { {0.8,0.8,0.8,0.8} ,{0.8,0.8,0.8,0.8},{0.8,0.8,0.8,0.8},{0.8,0.8,0.8,0.8} };

    float (*buttonBounds[8])[4];



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyBindingsComponent)
};
