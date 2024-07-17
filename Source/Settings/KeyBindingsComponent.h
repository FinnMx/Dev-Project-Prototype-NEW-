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
class KeyBindingsComponent : public juce::Component,
    public::juce::Button::Listener
{
public:
    KeyBindingsComponent();
    ~KeyBindingsComponent() override;

    void buttonClicked(juce::Button* button) override;

    bool isWaitingForBind();
    std::pair<int, int> getComponentAndAction();
    void setComponentAndActionFromButton(juce::Button* button);
    void resetBindWait();

    void setComponentAndAction(int component, int action);

    std::string* getHeadings();
    int* getParameterSizes();
    int getNumParameters();

    void refreshPage();

    void paint(juce::Graphics&) override;
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
    juce::TextButton tenBandParameters[12];
    juce::TextButton dubSirenParameters[4];
    juce::TextButton reverbParameters[3];
    juce::TextButton delayParameters[7];
    juce::TextButton killEQParameters[4];
    int parameterSizes[8]{ 5, 5, 2, 12, 4, 3, 7, 4 };
    int numParameters{ sizeof(headings) / sizeof(*headings) };

    juce::TextButton* parameters[8];

    float inputAButtonBounds[5][4] = { {0.003, 0.075,0.02,0.04} , {0.003,0.12,0.02,0.04}, {0.003,0.165,0.02,0.04}, {0.003,0.21,0.02,0.04}, {0.078,0.075,0.02,0.2} };
    float inputBButtonBounds[5][4] = { {0.128,0.075,0.02,0.04} ,{0.128,0.12,0.02,0.04},{0.128,0.165,0.02,0.04},{0.128,0.21,0.02,0.04},{0.203,0.075,0.02,0.2} };
    float thumbnailButtonBounds[2][4] = { {0.,0.,0.,0.} ,{0.,0.,0.,0.} };

    float tenBandButtonBounds[12][4] = { {0.778,0.085,0.02,0.2} ,{0.8,0.085,0.02,0.2},{0.822,0.085,0.02,0.2},{0.844,0.085,0.02,0.2},{0.866,0.085,0.02,0.2},
                                         {0.888,0.085,0.02,0.2} ,{0.91,0.085,0.02,0.2},{0.932,0.085,0.02,0.2},{0.954,0.085,0.02,0.2},{0.976,0.085,0.02,0.2}, {0.7725,0.34,0.04,0.04} ,{0.965,0.34,0.04,0.04} };

    float dubSirenButtonBounds[4][4] = { {0.012,0.495,0.05,0.075} ,{0.018,0.495,0.05,0.075} ,{0.03,0.625,0.015,0.025},{0.087,0.63,0.05,0.075} };

    float reverbButtonBounds[3][4] = { {0.362,0.42,0.075,0.1} , {0.512,0.42,0.075,0.1}, {0.662,0.42,0.075,0.1} };

    float delayButtonBounds[7][4] = { {0.288,0.555,0.015,0.025} ,{0.288,0.63,0.015,0.025} ,{0.362,0.588,0.075,0.1} , {0.512,0.588,0.075,0.1}, {0.6435,0.6,0.04,0.06},
                                      {0.7175,0.6,0.04,0.06} ,{0.774,0.615,0.015,0.025} };
    float killEQButtonBounds[4][4] = { {0.3,0.905,0.015,0.025} ,{0.45,0.905,0.015,0.025},{0.6,0.905,0.015,0.025},{0.75,0.905,0.015,0.025} };

    float(*buttonBounds[8])[4];



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyBindingsComponent)
};