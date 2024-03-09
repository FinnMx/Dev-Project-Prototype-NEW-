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

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    bool waitingForBind{ false };
    std::pair<int, int> currentComponentAndAction{ std::make_pair(NULL, NULL) };

    std::string headings[8] = { "Input A Bindings", "Input B Bindings", "Thumbnail Bindings", "10 Band Bindings","Dub Siren Bindings", "Reverb Bindings", "Delay Bindings", "Kill EQ Bindings" };
    juce::Label headingsLabels[8];
    float headingLabelsX[8] = { 0.005, 0.005, 0.005, 0.355, 0.355, 0.705, 0.705, 0.705 };
    float headingLabelsY[8] = { 0.005, 0.355, 0.655, 0.005, 0.555, 0.005, 0.355, 0.655 };

    //the index of the array is the component and the value is the amount of parameters
    int parameterAmounts[8] = { 5,5,2,10,3,3,4,4 };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyBindingsComponent)
};
