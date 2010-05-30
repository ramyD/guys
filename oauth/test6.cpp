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

int main(int argc, char** argv)
{
	std::string accessTokenString;
	std::string token;
	std::string tokenSecret;
	
	Oauth *myOauth = new Oauth( "Oe8TiUN5SlfY2mRGKiRlxw", 
								"fA4iyWCQL5oR2ArYNYHmJ7THZH2BOmdHGEv6XsUOKI", 
								"oob", 
								"POST", 
								"HMAC-SHA1",
								"https://api.twitter.com/oauth/request_token");
	accessTokenString = myOauth->requestAccessToken();
	 
	token = format::extract("oauth_token", '&', accessTokenString); 
	tokenSecret = format::extract("oauth_token_secret", '&', accessTokenString); 
	
	
	std::cout << std::endl << "token: " << token << std::endl;
	std::cout << "token secret: " << tokenSecret << std::endl;
	
	return 0;
}
