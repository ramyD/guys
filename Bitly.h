#ifndef BITLY_H
#define BITLY_H

#include <string>
#include <sstream>
#include <vector>

#include <curl/curl.h>
#include "tinyxml.h"
#include "format.h"

class Bitly{
public:
    Bitly();
	std::string shorten(std::string uri); //shorten a URL
    std::string checkForUrl(std::string fullString); //check the passed string for containing a URL

	std::string userlogin;
	std::string apiKey;
    std::string fullUrl;
	std::string apiPageBuffer;
    
    std::string urlStarts[2];
    //std::string urlContains[6];

	CURL *curl;
	CURLcode bitlyResult;
};

#endif
