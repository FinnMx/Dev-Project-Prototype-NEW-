/*
  ==============================================================================

    KeyBindingsComponent.cpp
    Created: 4 Mar 2024 4:11:59pm
    Author:  New Owner

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KeyBindingsComponent.h"

//==============================================================================
KeyBindingsComponent::KeyBindingsComponent()
{
    currentHeading = headings[0];

    parameters[0] = inputAParameters;
    parameters[1] = inputBParameters;
    parameters[2] = thumbnailParameters;
    parameters[3] = tenBandParameters;
    parameters[4] = dubSirenParameters;
    parameters[5] = reverbParameters;
    parameters[6] = delayParameters;
    parameters[7] = killEQParameters;
    buttonBounds[0] = inputAButtonBounds;
    buttonBounds[1] = inputBButtonBounds;
    buttonBounds[2] = thumbnailButtonBounds;
    buttonBounds[3] = tenBandButtonBounds;
    buttonBounds[4] = dubSirenButtonBounds;
    buttonBounds[5] = reverbButtonBounds;
    buttonBounds[6] = delayButtonBounds;
    buttonBounds[7] = killEQButtonBounds;

    refreshPage();
}

KeyBindingsComponent::~KeyBindingsComponent()
{

}

void KeyBindingsComponent::refreshPage() {
    removeAllChildren();
    currentHeading = headings[currentTab];
    repaint();
    addAndMakeVisible(pageForward);
    addAndMakeVisible(pageBackwards);
    pageForward.addListener(this);
    pageBackwards.addListener(this);
     
    for (int i = 0; i < sizeof(parameters) / sizeof(*parameters); i++) {
        for (int j = 0; j < parameterSizes[i]; j++)
        {
            addAndMakeVisible(parameters[i][j]);
            parameters[i][j].addListener(this);
            parameters[i][j].setColour(juce::TextButton::buttonColourId, (juce::Colours::lawngreen));
            parameters[i][j].setAlpha(0.6f);
        }
    }
}

void KeyBindingsComponent::buttonClicked(juce::Button* button) {
        const juce::MessageManagerLock mmLock;
        waitingForBind = true;
        setComponentAndActionFromButton(button);
        repaint();
        // refreshPage () change this func so when called buttons change colour etc...
}

bool KeyBindingsComponent::isWaitingForBind() {
    return waitingForBind;
}

std::pair<int, int> KeyBindingsComponent::getComponentAndAction() {
    return currentComponentAndAction;
}

void KeyBindingsComponent::setComponentAndActionFromButton(juce::Button* button) {
    for (int i = 0; i < sizeof(parameters) / sizeof(*parameters); i++) {
        for (int j = 0; j < parameterSizes[i]; j++) {
            if (button == &parameters[i][j])
                setComponentAndAction(i + 1, j);
        }
    }
}

int KeyBindingsComponent::findIndex(juce::TextButton arr[], juce::Button* button) {
    int index = 0;
    while (button != &arr[index]) {
        index++;
    }
    return index;
}

int KeyBindingsComponent::getActionIndex(juce::Button* button) {
    return findIndex(parameters[currentTab], button);

}

void KeyBindingsComponent::setComponentAndAction(int component, int action) {
    currentComponentAndAction.first = component;
    currentComponentAndAction.second = action;
}

void KeyBindingsComponent::resetBindWait() {
    const juce::MessageManagerLock mmLock;
    waitingForBind = false;
    repaint();
}

void KeyBindingsComponent::paint (juce::Graphics& g)
{

    g.setColour(juce::Colours::white);
    g.setFont(30.0f);
   // g.drawText(currentHeading, getLocalBounds(),
    //    juce::Justification::centredTop, true);
    
    if (waitingForBind) {
        g.setColour(juce::Colours::black);
        g.setFont(36.0f);
        g.drawText("Waiting for a input", getLocalBounds(),
            juce::Justification::centredBottom, true);
    }
}

void KeyBindingsComponent::resized()
{

    for (int i = 0; i < sizeof(parameters) / sizeof(*parameters); i++) {
        for (int j = 0; j < parameterSizes[i]; j++) {
            parameters[i][j].setBounds(getWidth() * buttonBounds[i][j][0], getHeight() * buttonBounds[i][j][1], getWidth() * buttonBounds[i][j][2], getHeight() * buttonBounds[i][j][3]);
        }
    }

    /*
    for (int i = 0; i < sizeof(parameters) / sizeof(*parameters); i++) {
        for (int j = 0; j < parameterSizes[i]; j++) {
            parameters[i][j].setBounds(getWidth() * 0.1, getHeight() * parameterY[j], getWidth() * 0.15, getHeight() * 0.05);
        }
    }
    */

}
