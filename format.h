#ifndef FORMAT_H
#define FORMAT_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <set>

#include <crypto++/cryptlib.h>
#include <crypto++/sha.h>
#include <crypto++/hmac.h>
#include <crypto++/base64.h>

#include <curl/curl.h>

class format {
public:
	static std::string encode(std::string stringToEncode, CURL *curl); //only encode reserved set, encoding must be in all caps
	static std::vector <std::string> sortVector(std::vector <std::string> vectorToSort, char tailCharacter);
	static std::string vectorToString(std::vector <std::string> stringToPrint);
	static std::string encrypt(std::string inputText, std::string inputKey);
	static std::string replaceInString(std::string stringToSwap, const std::string find, std::string replace);
	static std::string extract(std::string startString, char delimitingCharacter, std::string fullString);

	static int writer(char *data, size_t size, size_t nmemb, std::string *buffer); //This is the writer call back function used by curl
	static void dump(const char *text, FILE *stream, unsigned char *ptr, size_t size, char nohex);
	static int my_trace(CURL *handle, curl_infotype type, unsigned char *data, size_t size, void *userp);

private:
	static bool isCharacterReserved(char characterToEvaluate);
	static int calculateBase64Size(int digestSize);
};

#endif
