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
#include <fstream>
#include <windows.h>
#include <Shlobj.h>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <Settings/KeyBindingsComponent.h>

//==============================================================================
/*
*/
class MidiHandler  : public juce::Component
{
public:
    MidiHandler(KeyBindingsComponent* keyBindingsComponent);
    ~MidiHandler() override;

    void readSettingsFile();
    void saveSettingsFile();

    void bindKey(int key, int Component, int action);
    int returnCorrespondingComponent(int key);
    int returnCorrespondingAction(int key);
    int returnCorrespondingKey(int Component, int action);

private:
    void processSettings(rapidjson::Document& document);
    void saveSettings(rapidjson::Document& document);
    void resetSettingsFile();

#ifdef _WIN32
    char* filePath{ "C:/midisettings.json" };

#elif __APPLE__
    const char* filePath{ "C:/midisettings.json" };

#endif

    KeyBindingsComponent* keyBindingsComponent;

    std::unordered_map<int, std::pair<int, int>> bindings;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiHandler)
};
