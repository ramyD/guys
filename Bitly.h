#ifndef BITLY_H
#define BITLY_H

#include <string>
#include <sstream>
#include <vector>

#include <curl/curl.h>
#include "tinyxml.h"

class Bitly{
public:
	Bitly();
    int interpretArguments(int argc, char* argv[]);
	void shorten(std::string uri); //shorten a URL

private:
	std::string userlogin;
	std::string apiKey;
    std::string fullUrl;
	std::string apiPageBuffer;

	CURL	*curl;
	CURLcode bitlyResult;
};

#endif
