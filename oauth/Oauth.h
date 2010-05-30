#ifndef OAUTH_H
#define OAUTH_H

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

#include <curl/curl.h>

class Oauth {
public:
	Oauth(std::string cKey, std::string cSecret, std::string cbURL, std::string hMethod, std::string authMethod, std::string rtURL);
	std::string requestAccessToken();
private:
	std::string getTime();
	std::string getNoonce();
	std::string getBaseString();
	std::string getSignatureBaseString(std::string baseString);
	std::string post(std::string postData, struct curl_slist *headers=NULL, std::string url=" "); //request body with a  content-type  of application/x-www-form-urlencoded

	CURL *curl;

	std::string httpMethod;
	std::string requestTokenUrl;
	std::string consumerKey;
	std::string consumerSecret;
	std::string signatureMethod;
	std::string timeStamp;
	std::string noonce;
	std::string oauthVersion;
	std::string callBack;
};

#endif
