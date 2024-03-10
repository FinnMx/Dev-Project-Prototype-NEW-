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

    switch (currentTab) {
    case 0:
        for each (juce::TextButton & button in inputAParameters) { addAndMakeVisible(button); button.addListener(this); button.setButtonText("Edit Parameter"); }
        break;
    case 1:
        for each (juce::TextButton & button in inputBParameters) { addAndMakeVisible(button); button.addListener(this); }
        break;
    case 2:
        for each (juce::TextButton & button in thumbnailParameters) { addAndMakeVisible(button); button.addListener(this); }
        break;
    case 3:
        for each (juce::TextButton & button in tenBandParameters) { addAndMakeVisible(button); button.addListener(this); }
        break;
    case 4:
        for each (juce::TextButton & button in dubSirenParameters) { addAndMakeVisible(button); button.addListener(this); }
        break;
    case 5:
        for each (juce::TextButton & button in reverbParameters) { addAndMakeVisible(button); button.addListener(this); }
        break;
    case 6:
        for each (juce::TextButton & button in delayParameters) { addAndMakeVisible(button); button.addListener(this); }
        break;
    case 7:
        for each (juce::TextButton & button in killEQParameters) { addAndMakeVisible(button); button.addListener(this); }
        break;
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
    switch (currentTab) {
    case 0:
        return findIndex(inputAParameters, button);
        break;
    case 1:
        return findIndex(inputBParameters, button);
        break;
    case 2:
        return findIndex(thumbnailParameters, button);
        break;
    case 3:
        return findIndex(tenBandParameters, button);
        break;
    case 4:
        return findIndex(dubSirenParameters, button);
        break;
    case 5:
        return findIndex(reverbParameters, button);
        break;
    case 6:
        return findIndex(delayParameters, button);
        break;
    case 7:
        return findIndex(killEQParameters, button);
        break;
    }
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
    //NONE OF THIS WILL EVER RUN MORE THAN ONCE BECAUSE THE WINDOW CANNOT BE RESIZED!!

    pageForward.setBounds(getWidth() * 0.9,
        getHeight() * 0.1,
        getWidth() * 0.05,
        getHeight() * 0.05);

    pageBackwards.setBounds(getWidth() * 0.85,
        getHeight() * 0.1,
        getWidth() * 0.05,
        getHeight() * 0.05);

    for (int i = 0; i <= (sizeof(inputAParameters) / sizeof(*inputAParameters)); i++) { inputAParameters[i].setBounds(getWidth() * 0.1, getHeight() * parameterY[i], getWidth() * 0.15, getHeight() * 0.05); }
    for (int i = 0; i <= (sizeof(inputBParameters) / sizeof(*inputBParameters)); i++){ inputBParameters[i].setBounds(getWidth() * 0.1, getHeight() * parameterY[i], getWidth() * 0.15, getHeight() * 0.05); }
    for (int i = 0; i <= (sizeof(thumbnailParameters) / sizeof(*thumbnailParameters)); i++) { thumbnailParameters[i].setBounds(getWidth() * 0.1, getHeight() * parameterY[i], getWidth() * 0.15, getHeight() * 0.05); }
    for (int i = 0; i <= (sizeof(tenBandParameters) / sizeof(*tenBandParameters)); i++) { tenBandParameters[i].setBounds(getWidth() * 0.1, getHeight() * parameterY[i], getWidth() * 0.15, getHeight() * 0.05); }
    for (int i = 0; i <= (sizeof(dubSirenParameters) / sizeof(*dubSirenParameters)); i++) { dubSirenParameters[i].setBounds(getWidth() * 0.1, getHeight() * parameterY[i], getWidth() * 0.15, getHeight() * 0.05); }
    for (int i = 0; i <= (sizeof(reverbParameters) / sizeof(*reverbParameters)); i++) { reverbParameters[i].setBounds(getWidth() * 0.1, getHeight() * parameterY[i], getWidth() * 0.15, getHeight() * 0.05); }
    for (int i = 0; i <= (sizeof(delayParameters) / sizeof(*delayParameters)); i++) { delayParameters[i].setBounds(getWidth() * 0.1, getHeight() * parameterY[i], getWidth() * 0.15, getHeight() * 0.05); }
    //FOR SOME REASON THE SIZE OF THING DOESNT WORK??!?!
    for (int i = 0; i <= 3; i++) { killEQParameters[i].setBounds(getWidth() * 0.1, getHeight() * parameterY[i], getWidth() * 0.15, getHeight() * 0.05); }

}
