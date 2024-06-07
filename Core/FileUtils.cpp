#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <fstream>

int FileReadToBuffer(std::vector<uint8_t>& outBuffer, const std::string& fileName)
{
	FILE* fp;
 	errno_t er = fopen_s(&fp, fileName.c_str(), "rt");
	UNUSED(er);

	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END);

		long byteSize = ftell(fp);
		outBuffer.resize((size_t)byteSize);

		fseek(fp, 0, SEEK_SET); 
		fread(outBuffer.data(), sizeof(outBuffer[0]), byteSize, fp);

		fclose(fp);
	}
	else 
	{
		return -1;
	}

	return (int)outBuffer.size();

}

int FileReadToString(std::string& outString, const std::string& fileName)
{
	std::vector<uint8_t> outBuffer;

	FileReadToBuffer(outBuffer, fileName);

	for (size_t i = 0; i < outBuffer.size(); ++i)
	{
		outString.push_back(static_cast<char>(outBuffer[i]));
	}

	return (int)outBuffer.size();
}

int FileReadToBinary(std::vector<uint8_t>& outBuffer, const std::string& fileName)
{
	FILE* fp;
	errno_t er = fopen_s(&fp, fileName.c_str(), "rb");
	UNUSED(er);

	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END);

		long byteSize = ftell(fp);
		outBuffer.resize((size_t)byteSize);

		fseek(fp, 0, SEEK_SET);
		fread(outBuffer.data(), sizeof(outBuffer[0]), byteSize, fp);

		fclose(fp);
	}
	else
	{
		return -1;
	}

	return (int)outBuffer.size();
}

int FileWriteBinary(std::string const& fileName, std::vector<unsigned char>fileContent)
{
	std::ofstream outputFileStream;
	outputFileStream.open(fileName, std::ios::out | std::ios::binary);
	outputFileStream.write((char*)fileContent.data(), fileContent.size());
	outputFileStream.close();
	return 0;
}

bool CreateFolder(std::string filePath)
{
	std::wstring wsr = std::wstring(filePath.begin(), filePath.end());

	return CreateDirectory(wsr.c_str(), NULL);
}
