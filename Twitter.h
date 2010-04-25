#ifndef TWITTER_H
#define TWITTER_H

#include <string>
#include <curl/curl.h>
#include "tinyxml.h"

class Twitter{
public:
	Twitter();
	void		post(std::string message);  //posts to twitter using basic auth
	void		getTimeline(void); //gets the last 20 tweets using basic auth

private:
	const char	*getIndent( unsigned int numIndents ); //for the xml from the timeline
	void		dump_to_stdout( TiXmlNode * pParent, unsigned int indent = 0 ); //debugging function to see raw xml data
	std::string		UserName;
	std::string		Password;
	std::string		Account;
	std::string		URL;
	std::string		apiPageBuffer;


	CURL		*curl;
	CURLcode	twitterResult;
};

#endif
