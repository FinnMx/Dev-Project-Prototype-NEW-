#include "SettingsHandler.h"


SettingsHandler::SettingsHandler() {
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath))) {
        std::string wstrPath(szPath);
        std::string strPath(wstrPath.begin(), wstrPath.end());
        std::replace(strPath.begin(), strPath.end(), '\\', '/');
        dirPath = strPath + "/3amp";
        dubSirenSettingsFilePath = dirPath + "/dubsirensettings.txt";
    }
    else {
        dubSirenSettingsFilePath = ""; // Return empty string if failed to get path
    }

    if (!IfFileExists(dubSirenSettingsFilePath))
        CreateDefaultDubSirenSettings(dubSirenSettingsFilePath);

}

SettingsHandler::~SettingsHandler() {

}

void SettingsHandler::SaveDubSirenSettings(const SettingsHandler::Settings& settings) {
    WriteDubSirenSettings(dubSirenSettingsFilePath, settings);
}

SettingsHandler::Settings SettingsHandler::LoadDubSirenSettings() {
    return ReadDubSirenSettings(dubSirenSettingsFilePath);
}

bool SettingsHandler::IfFileExists(const std::string& fileName) {
	std::ifstream infile(fileName);
	return infile.good();
}

void SettingsHandler::CreateDefaultDubSirenSettings(const std::string& fileName) {
    std::ofstream outfile(fileName);
    for (int i = 0; i < DUBSIREN_NUM_PRESETS; ++i) {
        outfile << "100,25,1,2,80\n";  // Default values
    }
    outfile.close();
}

SettingsHandler::Settings SettingsHandler::ReadDubSirenSettings(const std::string& filename) {
    Settings settings(DUBSIREN_NUM_PRESETS);
    std::ifstream infile(filename);
    std::string line;
    int lineNumber = 0;

    while (std::getline(infile, line) && lineNumber < DUBSIREN_NUM_PRESETS) {
        std::istringstream iss(line);
        std::string value;
        int valueNumber = 0;

        while (std::getline(iss, value, ',') && valueNumber < DUBSIREN_NUM_VALUES) {
            settings[lineNumber][valueNumber] = std::stoi(value);
            ++valueNumber;
        }
        ++lineNumber;
    }
    infile.close();
    return settings;
}

void SettingsHandler::WriteDubSirenSettings(const std::string& filename, const SettingsHandler::Settings& settings) {
    std::ofstream outfile(filename);
    for (const auto& line : settings) {
        for (size_t i = 0; i < line.size(); ++i) {
            outfile << line[i];
            if (i < line.size() - 1) {
                outfile << ",";
            }
        }
        outfile << "\n";
    }
    outfile.close();
}