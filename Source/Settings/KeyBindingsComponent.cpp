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
     
    for (int i = 0; i < parameterSizes[currentTab]; i++)
    {
        addAndMakeVisible(parameters[currentTab][i]);
        parameters[currentTab][i].addListener(this);
        parameters[currentTab][i].setButtonText("Edit Parameter");
    }
}

void KeyBindingsComponent::buttonClicked(juce::Button* button) {
    if (button == &pageForward) {
        if (currentTab == 7)
            currentTab = -1;
        currentTab++;
        refreshPage();
        return;
    }
    if (button == &pageBackwards) {
        if (currentTab == 0)
            currentTab = 8;
        currentTab--;
        refreshPage();
        return;
    }
    else {
        const juce::MessageManagerLock mmLock;
        waitingForBind = true;
        setComponentAndAction(currentTab, button);
        repaint();
    }
}

bool KeyBindingsComponent::isWaitingForBind() {
    return waitingForBind;
}

std::pair<int, int> KeyBindingsComponent::getComponentAndAction() {
    return currentComponentAndAction;
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

void KeyBindingsComponent::setComponentAndAction(int component, juce::Button* button) {
    currentComponentAndAction.first = component;
    currentComponentAndAction.second = getActionIndex(button);
}

void KeyBindingsComponent::resetBindWait() {
    const juce::MessageManagerLock mmLock;
    waitingForBind = false;
    repaint();
}

void KeyBindingsComponent::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::PropertyComponent::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(30.0f);
    g.drawText(currentHeading, getLocalBounds(),
        juce::Justification::centredTop, true);
    
    if (waitingForBind) {
        g.setColour(juce::Colours::white);
        g.setFont(16.0f);
        g.drawText("Waiting for a input", getLocalBounds(),
            juce::Justification::centredBottom, true);
    }
}

void KeyBindingsComponent::resized()
{

    pageForward.setBounds(getWidth() * 0.9,
        getHeight() * 0.1,
        getWidth() * 0.05,
        getHeight() * 0.05);

    pageBackwards.setBounds(getWidth() * 0.85,
        getHeight() * 0.1,
        getWidth() * 0.05,
        getHeight() * 0.05);


    for (int i = 0; i < sizeof(parameters) / sizeof(*parameters); i++) {
        for (int j = 0; j < parameterSizes[i]; j++) {
            parameters[i][j].setBounds(getWidth() * 0.1, getHeight() * parameterY[j], getWidth() * 0.15, getHeight() * 0.05);
        }
    }

}
