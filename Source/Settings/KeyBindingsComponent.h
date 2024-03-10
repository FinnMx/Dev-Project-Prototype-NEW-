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
    float parameterY[10]{ 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55,0.6, 0.65 };



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyBindingsComponent)
};
