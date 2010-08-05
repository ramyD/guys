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
#include "data.h"

std::string parsePage(std::string token, std::string twitterURL) {
	std::cout << "<<< parsing twitter login page: ";
	CURL *curl;
	curl = curl_easy_init();
	CURLcode curlResult;
	std::string twitterPageBuffer;
	std::string url = twitterURL + "?oauth_token=" + token;
	std::string cookies = "cookies.txt";
	
	std::cout << url << std::endl;
	
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, format::writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &twitterPageBuffer);
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookies.c_str() );
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str() );
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
	curlResult = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	
	return twitterPageBuffer;
}

std::string getTwitterFormAction(std::string *twitterLoginPage) {
	std::cout << "<<< getting twitter form action" << std::endl;
	std::string tagForm = "<form action=\"";
	size_t tagBegin, tagEnd;
	tagBegin = twitterLoginPage->find(tagForm);
	tagBegin += tagForm.length(); //puts the tag after the first "
	tagEnd = twitterLoginPage->find_first_of("\"", tagBegin+1); //finds the end "
	
	//at this point, tagBegin and tagEnd have the inclusive position of the string we want, always counting from the begining.
	
	return twitterLoginPage->substr(tagBegin, tagEnd - tagBegin); //substring take the first parameter as a position (count from 0) and a length (count from 1) which requires substraction
}

std::string getTwitterAuthenticityToken(std::string *twitterLoginPage) {
	std::cout << "<<< getting twitter authenticity token" << std::endl;
	std::string tagInput = "<input name=\"authenticity_token\" type=\"hidden\" value=\"";
	size_t tagBegin, tagEnd;
	tagBegin = twitterLoginPage->find(tagInput);
	tagBegin += tagInput.length(); //puts the tag after the first "
	tagEnd = twitterLoginPage->find_first_of("\"", tagBegin+1); //finds the end "
	
	//at this point, tagBegin and tagEnd have the inclusive position of the string we want, always counting from the begining.
	
	return twitterLoginPage->substr(tagBegin, tagEnd - tagBegin); //substring take the first parameter as a position (count from 0) and a length (count from 1) which requires substraction;
}

std::string getUserName() {
	std::string message;
	std::cout << "please enter your twitter username or email: " ;
	getline (std::cin, message);
	
	std::cout << "username debug: " << message << " user length: " << message.length() << std::endl;
	
	return message;
}

std::string getPassword() {
	std::cout << "please enter your twitter password: ";
	char passwordChar;
	std::stringstream passwordStream;
	std::string password;
	
	do {
		passwordChar = getchar();
		passwordStream << passwordChar;
		putchar ('*');
	} while (passwordChar != '\n');
	
	password = passwordStream.str();
	password.erase(password.end()-1 ); //remove the last character which is "enter"
	
	std::cout << "password debug: " << password << " password length: " << password.length() << std::endl;
	
	return passwordStream.str();
}

std::string getCode(std::string postData, std::string postUrl) {
	std::cout << "<<< getting pin code from " << postUrl << std::endl;
	CURL *curl;
	curl = curl_easy_init();
	CURLcode curlResult;
	std::string twitterPageBuffer;
	std::string url = postUrl;
	std::string cookies = "cookies.txt";
	
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, format::writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &twitterPageBuffer);
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookies.c_str() );
	curl_easy_setopt(curl, CURLOPT_URL, postUrl.c_str() );
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str() );
	curl_easy_setopt(curl, CURLOPT_HEADER, 0);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curlResult = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	
	return twitterPageBuffer;
}

std::string getPinNumber(std::string &twitterCodePage) {
	//TODO i need to see this page in order to know what to parse
	
	return "0";
}

std::string hackTwitter(std::string token) {
	std::string twitterLoginPage;
	std::string formAction;
	std::string authenticityToken;
	std::string userName;
	std::string password;
	std::string twitterPinPage;
	std::string twitterPinNumber;
	
	std::string postUrl = "https://api.twitter.com/oauth/authorize";
	std::string postData;
	std::string postAuthenticityToken, postOauthToken, postSessionEmailUser, postSessionPassword;
	postAuthenticityToken = "authenticity_token";
	postOauthToken = "oauth_token";
	postSessionEmailUser = "session%5Busername_or_email%5D";
	postSessionPassword = "session%5Bpassword%5D";
	
	twitterLoginPage = parsePage(token, postUrl);
	
	std::cout << "twitter login page %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl << twitterLoginPage << std::endl;
	
	formAction = getTwitterFormAction(&twitterLoginPage);
	
	std::cout << "form action is: " << formAction << std::endl;
	
	authenticityToken = getTwitterAuthenticityToken(&twitterLoginPage);
	userName = getUserName();
	password = getPassword();
	
	postData = postAuthenticityToken + '=' + authenticityToken + '&' + postOauthToken + '=' + token + '&' + postSessionEmailUser + '=' + userName + '&' + postSessionPassword + '=' + password;
	
	std::cout << "post data is: " << postData << std::endl;
	
	twitterPinPage = getCode(postData, formAction);
	//twitterPinNumber = getPinNumber(&twitterPinPage);
	
	//return twitterPinNumber;
	return twitterPinPage;
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
	 
	token = format::extract("oauth_token", '&', requestTokenString); 
	tokenSecret = format::extract("oauth_token_secret", '&', requestTokenString); 
	
	std::cout << std::endl << "token: " << token << std::endl;
	std::cout << "token secret: " << tokenSecret << std::endl;
	
	pinCode = hackTwitter(token);
	
	std::cout << "pin code page ####################" << std::endl << pinCode << std::endl;
	
	//accessTokenString = myOauth->requestAccessToken(token, tokenSecret, "https://api.twitter.com/oauth/authorize");
	
	return 0;
}
