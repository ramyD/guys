#ifndef TWITTER_H
#define TWITTER_H

#include <string>
#include <curl/curl.h>
#include "tinyxml.h"
#include "Bitly.h"
#include "Oauth.h"

class Twitter{
public:
	Twitter();
	void		post(std::string message, std::string configFilePath);  //posts to twitter using basic auth
	void		getTimeline(void); //gets the last 20 tweets using basic auth

private:
	const char	*getIndent( unsigned int numIndents ); //for the xml from the timeline
	void		dump_to_stdout( TiXmlNode * pParent, unsigned int indent = 0 ); //debugging function to see raw xml data
	std::string		UserName;
	std::string		Password;
	std::string		Account;
	std::string		URL;
	std::string		apiPageBuffer;

	static int twitterWriter(char *data, size_t size, size_t nmemb, std::string *buffer);

	Bitly *bitly;
	Oauth *myOauth;
	std::string configPath;
	
	std::string parsePage(std::string token, std::string twitterURL, std::string *resultUrl);
	std::string getTwitterFormAction(std::string *twitterLoginPage);
	std::string getTwitterAuthenticityToken(std::string *twitterLoginPage);
	std::string getPassword();
	std::string getCode(std::string postData, std::string postUrl, std::string refererUrl);
	std::string getPinNumber(std::string *twitterCodePage);
	std::string cleanPinResult(std::string pinNumber);
	std::string hackTwitter(std::string token);
	void promptUsername();
	void generateAccessToken();
	void writeFile();
	void readFile(std::string *token, std::string *tokenSecret);
	
};

#endif
