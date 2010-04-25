#ifndef FACEBOOK_H
#define FACEBOOK_H

//not paying too much attention to the Facebook side
//prolly want to build a timeline getter for the facebook site
//xpaths? *shudder*

#include <string>
#include <curl/curl.h>
#include <iostream>

class Facebook{
public:
	Facebook();
	void post(std::string message); //posts to facebook through facebook mobile

private:
	std::string FirstName;
	std::string Email;
	std::string Password;
	std::string loginPageBuffer;

	CURL	*curl;
	CURLcode facebookResult;
	
	//for frak sake we need to brake these down
	std::string LoginPost;
	std::string LoginPostEmail;
	std::string LoginPostPassword;
	std::string LoginPostLogin;
	
	std::string finalPageBuffer;
	std::string frontPageBuffer;
	std::string formKey;
	std::string actionURL;

	std::string finalURLp1;
	std::string finalURL;
	
	std::string	postFormId;
	std::string	postStatus;
	std::string	postUpdate;
	
	std::string Post;
	
	size_t foundFirst;
	size_t foundLast;
};

#endif
