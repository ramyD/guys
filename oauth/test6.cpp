//      test6.cpp
//      
//      Copyright 2010 Ramy d. <ramy.daghstani@gmail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

///g++ -Wall -std=c++98 -pedantic -Os test6.cpp Oauth.cpp format.cpp -o test6 -lcurl -lcrypto++
#include "Oauth.h"
#include "format.h"

std::string parsePage(std::string token, std::string twitterURL) {
	CURL *curl;
	CURLcode curlResult;
	std::string twitterPageBuffer
	std::string url = twitterURL + token;
	
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, format::writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &twitterPageBuffer);
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str() );
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
	curlResult = curl_easy_perform(curl);
	
	return twitterPageBuffer;
}

std::string getTwitterFormAction(std::string &twitterLoginPage) {
	std::string tagForm = "<form action=\""
	size_t tagBegin, tagEnd;
	tagBegin = twitterLoginPage.find(tagForm);
	tagBegin += tagForm.length(); //puts the tag after the first "
	tagEnd = twitterLoginPage.find_first_of("\"", tagBegin+1); //finds the end "
	tagEnd -= 1; //puts the tag right before the "
	
	//at this point, tagBegin and tagEnd have the inclusive position of the string we want, always counting from the begining.
	
	return twitterLoginPage.substr(tagBegin, tagEnd - tagBegin); //substring take the first parameter as a position (count from 0) and a length (count from 1) which requires substraction
}

std::string getTwitterAuthenticityToken(&twitterLoginPage) {
	std::string tagInput = "<input name=\"authenticity_token\" type=\"hidden\" value=\"";
	size_t tagBegin, tagEnd;
	tagBegin = twitterLoginPage.find(tagInput);
	tagBegin += tagInput.length(); //puts the tag after the first "
	tagEnd = twitterLoginPage.find_first_of("\"", tagBegin+1); //finds the end "
	tagEnd -= 1; //puts the tag right before the "
	
	//at this point, tagBegin and tagEnd have the inclusive position of the string we want, always counting from the begining.
	
	return twitterLoginPage.substr(tagBegin, tagEnd - tagBegin); //substring take the first parameter as a position (count from 0) and a length (count from 1) which requires substraction;
}

std::string getUserName() {
	std::cout << "please enter your twitter username or email: " ;
	getline (std::cin, message);
	return message;
}

std::string getPassword() {
	std::cout << "please enter your twitter password: ";
	int charCount;
	char passwordChar;
	stringstream passwordStream;
	do {
		passwordChar = getchar();
		if (passwordChar != 13 ) {
			passwordStream << passwordChar;
			std::cout << '*';
		}
	} while (passwordChar != 13 ); //13 is enter
	
	std::cout << std::endl;
	
	return passwordStream.str();
}

std::string getCode(std::string postData, std::string postUrl) {
	CURL *curl;
	CURLcode curlResult;
	std::string twitterPageBuffer
	std::string url = twitterURL + token;
	
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, format::writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &twitterPageBuffer);
	curl_easy_setopt(curl, CURLOPT_URL, postUrl.c_str() );
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str() );
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curlResult = curl_easy_perform(curl);
	
	return twitterPageBuffer;
}

std::string getPinNumber(std::string &twitterCodePage) {
	//TODO i need to see this page in order to know what to parse
}

std::string hackTwitter(std::string token) {
	std::string twitterLoginPage;
	std::string formAction;
	std::string authenticityToken;
	std::string userName;
	std::string password;
	std::string twitterPinPage;
	
	std::string postUrl = "https://api.twitter.com/oauth/authorize";
	std::string postData;
	std::string postAuthenticityToken, postOauthToken, postSessionEmailUser, postSessionPassword;
	postAuthenticityToken = "authenticity_token";
	postOauthToken = "oauth_token";
	postSessionEmailUser = "session[username_or_email]";
	postSessionPassword = "session[password]";
	
	twitterLoginPage = parsePage(token, postUrl);
	formAction = getTwitterFormAction(&twitterLoginPage);
	authenticityToken = getTwitterAuthenticityToken(&twitterLoginPage);
	userName = getUserName();
	password = getPassword();
	
	postData = postAuthenticityToken + '=' + authenticityToken + '&' + postOauthToken + '=' + token + '&' + postSessionEmailUser + '=' + userName + '&' + postSessionPassword + '=' + password;
	
	twitterPinPage = getPinPage(postData, postUrl);
	twitterPinNumber = getPinNumber(&twitterCodePage);
	
	return twitterPinNumber;
}

int main(int argc, char** argv) {
	std::string requestTokenString;
	std::string accessTokenString;
	std::string token;
	std::string tokenSecret;
	std::string pinCode;
	
	Oauth *myOauth = new Oauth( "Oe8TiUN5SlfY2mRGKiRlxw", 
								"fA4iyWCQL5oR2ArYNYHmJ7THZH2BOmdHGEv6XsUOKI", 
								"oob", 
								"POST", 
								"HMAC-SHA1");

	requestTokenString = myOauth->requestRequestToken("https://api.twitter.com/oauth/request_token");
	 
<<<<<<< HEAD:oauth/test6.cpp
	token = format::extract("oauth_token", '&', requestTokenString); 
	tokenSecret = format::extract("oauth_token_secret", '&', requestTokenString); 
=======
	token = format::extract("oauth_token=", '&', accessTokenString); 
	tokenSecret = format::extract("oauth_token_secret=", '&', accessTokenString); 
	
>>>>>>> 2e04f6e72f21c676c4a1765f920435b78640ae40:oauth/test6.cpp
	
	std::cout << std::endl << "token: " << token << std::endl;
	std::cout << "token secret: " << tokenSecret << std::endl;
	
	pinCode = hackTwitter(token);
	
	accessTokenString = myOauth->requestAccessToken(token, tokenSecret, "https://api.twitter.com/oauth/authorize");
	
	return 0;
}
