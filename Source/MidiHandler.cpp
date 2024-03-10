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
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        return;
    }
    std::string jsonString((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    rapidjson::Document document;
    document.Parse(jsonString.c_str());

    // Check if parsing was successful
    if (document.HasParseError()) {
        return;
    }
    processSettings(document);
}

void MidiHandler::processSettings(rapidjson::Document& document) {
    int componentIndex = 0; // (should technically be 0 to match the standard of the app but whatever JSON is stupid)
    for (rapidjson::Value::ConstMemberIterator it = document.MemberBegin(); it != document.MemberEnd(); ++it) {

        const rapidjson::Value& jsonArray = it->value;

        if (jsonArray.IsArray()) {
            for (rapidjson::SizeType i = 0; i < jsonArray.Size(); i++) {
                std::string key = jsonArray[i].GetString();
                if (key != "undefined") {
                    bindKey(std::stoi(key), componentIndex, i);
                }
            }
        }
        componentIndex++;
    }
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