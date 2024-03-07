/*
  ==============================================================================

    MidiHandler.h
    Created: 7 Mar 2024 12:06:47pm
    Author:  Finn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <iostream>
#include "rapidjson/document.h"
#include <fstream>

//==============================================================================
/*
*/
class MidiHandler  : public juce::Component
{
public:
    MidiHandler();
    ~MidiHandler() override;

    void readSettingsFile();

    void bindKey(int key, int Component, int action);
    int returnCorrespondingComponent(int key);
    int returnCorrespondingAction(int key);

private:
    const char* filePath{ "midisettings.json" };
    std::ifstream inFile{ filePath };

    rapidjson::Document document;
    std::unordered_map<int, std::pair<int, int>> bindings;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiHandler)
};
