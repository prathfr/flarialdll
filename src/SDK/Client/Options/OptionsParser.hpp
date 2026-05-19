#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include "../../Utils/Logger/Logger.hpp"

class OptionsParser
{
public:
	std::map<std::string, std::string> options;
    static std::filesystem::path getOptionsFilePath();
    std::map<std::string, std::string> parseOptionsFile();
};
