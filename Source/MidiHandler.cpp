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
MidiHandler::MidiHandler(KeyBindingsComponent* keyBindingsComponent) : keyBindingsComponent(keyBindingsComponent)
{
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath))) {
        std::string wstrPath(szPath);
        std::string strPath(wstrPath.begin(), wstrPath.end());
        std::replace(strPath.begin(), strPath.end(), '\\', '/');
        dirPath = strPath + "/3amp";
        filePath = dirPath + "/midisettings.json";
        createDirectoryIfNotExists();
    }
    else {
        filePath = ""; // Return empty string if failed to get path
    }
}

MidiHandler::~MidiHandler()
{
    saveSettingsFile();
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

    inFile.close();
}

void MidiHandler::createDirectoryIfNotExists() {
    CreateDirectory(dirPath.c_str(), NULL);

    HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_EXISTS) {
            DBG("File already exists.");
        }
    }
    else {
        std::cout << "File created successfully." << std::endl;
        CloseHandle(hFile);
    }
}

void MidiHandler::saveSettingsFile() {
    resetSettingsFile();
}

void MidiHandler::saveSettings(rapidjson::Document& document) {
    int componentIndex = 0;

}

void MidiHandler::processSettings(rapidjson::Document& document) {
    int componentIndex = 0;
    for (rapidjson::Value::ConstMemberIterator it = document.MemberBegin(); it != document.MemberEnd(); ++it) {

        const rapidjson::Value& jsonArray = it->value;

        if (jsonArray.IsArray()) {
            for (rapidjson::SizeType i = 0; i < jsonArray.Size(); i++) {
                int key = jsonArray[i].GetInt();
                if (key != 0) {
                    bindKey(key, componentIndex + 1, i);
                }
            }
        }
        componentIndex++;
    }
}

void MidiHandler::resetSettingsFile() {

    rapidjson::Document document;
    document.SetObject();

    std::string* componentNames = keyBindingsComponent->getHeadings();
    int* parameterSizes = keyBindingsComponent->getParameterSizes();

    for (int i = 0; i < keyBindingsComponent->getNumParameters(); i++) {
        rapidjson::Value tempVal(rapidjson::kArrayType);

        for (int j = 0; j < parameterSizes[i]; j++) {
            tempVal.PushBack(returnCorrespondingKey(i + 1, j), document.GetAllocator());
        }

        document.AddMember(rapidjson::GenericStringRef<char>(componentNames[i].data(), static_cast<rapidjson::SizeType>(componentNames[i].length())), tempVal, document.GetAllocator());
    }

    rapidjson::StringBuffer stringBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(stringBuffer);
    document.Accept(writer);

    const char* jsonStr = stringBuffer.GetString();

    std::ofstream outFile(filePath);
    if(outFile.is_open()){
        outFile << jsonStr;
        outFile.close();
    }
}

void MidiHandler::bindKey(int key, int Component, int action) {
    for (auto it = bindings.begin(); it != bindings.end(); )
    {
        if (it->second.first == Component && it->second.second == action) { bindings.erase(it++); }
        else { ++it; }
    }
    bindings[key] = std::make_pair(Component, action);
}

int MidiHandler::returnCorrespondingComponent(int key) {
    return bindings[key].first;
}

int MidiHandler::returnCorrespondingAction(int key) {
    return bindings[key].second;
}

int MidiHandler::returnCorrespondingKey(int Component, int action) {
    for (const auto& pair : bindings) {
        if (pair.second.first == Component && pair.second.second == action) {
            return pair.first;
        }
    }
    return 0;
}