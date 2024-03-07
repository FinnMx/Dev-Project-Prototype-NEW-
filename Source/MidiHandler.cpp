/*
  ==============================================================================

    MidiHandler.cpp
    Created: 7 Mar 2024 12:06:47pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MidiHandler.h"

//==============================================================================
MidiHandler::MidiHandler()
{
}

MidiHandler::~MidiHandler()
{
}

void MidiHandler::readSettingsFile() {

}

void MidiHandler::bindKey(int key, int Component, int action) {
    bindings[key] = std::make_pair(Component, action);
}

int MidiHandler::returnCorrespondingComponent(int key) {
    return bindings[key].first;
}

int MidiHandler::returnCorrespondingAction(int key) {
    return bindings[key].second;
}