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

    for (int i = 0; i < (sizeof(headingsLabels) / sizeof(*headingsLabels)); i++){
        headingsLabels[i].setText(headings[i], juce::NotificationType{});
        addAndMakeVisible(headingsLabels[i]);

        //NEED TO ADD BUTTONS TO START BIND
        for (int j = 0; j < parameterAmounts[i]; j++) {

        }
    }
}

KeyBindingsComponent::~KeyBindingsComponent()
{

}

void KeyBindingsComponent::buttonClicked(juce::Button* button) {

}

bool KeyBindingsComponent::isWaitingForBind() {
    return waitingForBind;
}

std::pair<int, int> KeyBindingsComponent::getComponentAndAction() {
    return currentComponentAndAction;
}

void KeyBindingsComponent::resetBindWait() {
    waitingForBind = false;
}

void KeyBindingsComponent::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::PropertyComponent::backgroundColourId));
}

void KeyBindingsComponent::resized()
{

    for (int i = 0; i < (sizeof(headingsLabels) / sizeof(*headingsLabels)); i++) {
            headingsLabels[i].setBounds(
                getWidth() * headingLabelsX[i],
                getHeight() * headingLabelsY[i],
                getWidth() * 0.2,
                getHeight() * 0.15);
    }
}
