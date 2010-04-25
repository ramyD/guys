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

private:
	float versionNumber;
};

#endif
