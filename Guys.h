#ifndef GUYS_H
#define GUYS_H

#include <string>
#include <sstream>
#include <vector>

#include "Twitter.h"
#include "Facebook.h"

class Guys {
public:
	Guys();
	int interpretArguments(int argc, char* argv[]);
	int displayHelp();
	int displayVersion();

	bool folderExists(std::string path);
	bool configFileExists(std::string path);
	void makeConfigFile(std::string fileName);
	void makeConfigDir(std::string folderName);
	void configurationFileCheck();
	void readFile();
	void generateKey();
	std::string configPath;

private:
	float versionNumber;
};

#endif
