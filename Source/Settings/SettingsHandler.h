#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <sstream>
#include <windows.h>
#include <Shlobj.h>
#include <Knownfolders.h>

#include <algorithm>
#include <complex>
#include <functional>
#pragma comment(lib, "Shell32.lib")

class SettingsHandler
{
public:
	SettingsHandler();
	~SettingsHandler();

	using Settings = std::vector<std::array<float, 4>>;

	void SaveDubSirenSettings(const SettingsHandler::Settings& settings);
	Settings LoadDubSirenSettings();

private:
	bool IfFileExists(const std::string& fileName);
	void CreateDefaultDubSirenSettings(const std::string& fileName);
	Settings ReadDubSirenSettings(const std::string& filename);
	void WriteDubSirenSettings(const std::string& filename, const Settings& settings);

	std::string dirPath;
	std::string dubSirenSettingsFilePath;

    const int DUBSIREN_NUM_PRESETS = 5;
	const int DUBSIREN_NUM_VALUES = 4;

};

