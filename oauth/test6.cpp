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
/*
std::string hackTwitter(std::string token) {
	std::string twitterLoginPage;
	std::string formAction;
	std::string authenticityToken;
	std::string userName;
	std::string password;
	std::string twitterCode;
	
	
	twitterLoginPage = parsePage(token, "https://api.twitter.com/oauth/authorize");
	formAction = getTwitterFormAction(&twitterLoginPage);
	authenticityToken = getTwitterAuthenticityToken(&twitterLoginPage);
	userName = getUserName(&twitterLoginPage);
	password = getPassword(&twitterLoginPage);
	
	twitterCode = getCode(formAction, authenticityToken, userName, password);
	
	return twitterCode;
}
*/
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

int main(int argc, char** argv)
{
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
	
	//pinCode = hackTwitter(token);
	
	accessTokenString = myOauth->requestAccessToken(token, tokenSecret, "https://api.twitter.com/oauth/authorize");
	
	
	
	
	return 0;
}
