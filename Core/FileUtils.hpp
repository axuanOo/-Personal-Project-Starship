#pragma once
#include <vector>
#include <string>

int FileReadToBuffer(std::vector<uint8_t>& outBuffer, const std::string& fileName);
int	FileReadToString(std::string& outString, const std::string& fileName);
int	FileReadToBinary(std::vector<uint8_t>& outBuffer, const std::string& fileName);
int	FileWriteBinary(std::string const& fileName, std::vector<unsigned char> fileContent);
bool CreateFolder(std::string filePath);