/*
 *      oauth.cpp
 *      
 *      Copyright 2009 Ramy d. <ramy.daghstani@gmail.com>
 *      
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#include "Oauth.h"
#include "format.h"
#include "data.h"

Oauth::Oauth(std::string cKey, std::string cSecret, std::string cbURL, std::string hMethod, std::string authMethod) {
	httpMethod = hMethod;
	consumerKey = cKey;
	consumerSecret = cSecret;
	signatureMethod = authMethod;
	oauthVersion = "1.0";
	callBack = cbURL;
	curl = curl_easy_init();
}


std::string Oauth::getTime() {
	time_t seconds;
	std::ostringstream temporaryStream;
	seconds = time (NULL);
	temporaryStream << seconds;
	return temporaryStream.str();
}

std::string Oauth::getNoonce() {
	std::ostringstream temporaryStream;
	char tempChar;
	char junk[] = {':',';','<','=','>','?','@','[','\\',']','^','_','`'}; //junk characters from '0' in decimal to 'z' in decimal
	std::set<char> junkCharacters(junk, junk + sizeof(junk) );
	const int noonceLength = 32;
	srand((unsigned) (clock () + time(NULL) * CLOCKS_PER_SEC ) ); //gives a seed based on time for the random function
	for (int i = 0; i < noonceLength; i++) {
		tempChar = (char) (rand() % 75 + '0'); //75 is the number of lowercase letters + uppercase letters + numbers from 0 to 9 + 13 junk letters in between
		if ( junkCharacters.find(tempChar) == junkCharacters.end() ) //find the temporary character in the set, if it is not found, proceede
			temporaryStream << (char) tempChar;
	}
	return temporaryStream.str();
}

std::string Oauth::getBaseString(std::string token, std::string pinCode) {
	std::vector <std::string> baseString;
	std::string returnString;
	baseString.push_back("oauth_consumer_key=" + format::encode( consumerKey.c_str(), curl ) + "&");
	baseString.push_back("oauth_signature_method=" + format::encode( signatureMethod.c_str(), curl ) + "&");
	baseString.push_back("oauth_timestamp=" + format::encode( timeStamp.c_str(), curl ) + "&");
	if( !token.empty() ) baseString.push_back("oauth_token=" + format::encode( token, curl ) + "&");
	baseString.push_back("oauth_nonce=" + format::encode( noonce.c_str(), curl ) + "&");
	if( !pinCode.empty() ) {
		baseString.push_back("oauth_verifier=" + format::encode( pinCode.c_str(), curl ) + "&");
	} else {
		baseString.push_back("oauth_callback=" + format::encode( callBack.c_str(), curl ) + "&");
	}
	baseString.push_back("oauth_version=" + format::encode( oauthVersion.c_str(), curl ) + "&");
	baseString = format::sortVector(baseString, '&');
	returnString = format::vectorToString(baseString);
	
	return returnString;
}

std::string Oauth::getSignatureBaseString(std::string baseString) {
	std::vector <std::string> signatureBaseString;
	std::string returnString;
	signatureBaseString.push_back(format::encode( httpMethod.c_str(), curl ) + "&");
	signatureBaseString.push_back(format::encode( requestUrl.c_str(), curl ) + "&");
	signatureBaseString.push_back(format::encode( baseString, curl ) );
	returnString = format::vectorToString(signatureBaseString);

	std::cout << "signatureBaseString for signature: " << returnString << std::endl << std::endl;

	return returnString;
}

std::string Oauth::requestRequestToken(std::string url) {
	std::vector <std::string> basicHeader;
	struct curl_slist *headers=NULL;
	std::string key;
	std::string postData;
	std::string headerData;
	std::string signatureBaseString;
	std::string signature;
	std::string postReturn;
	
	
	timeStamp = getTime();
	noonce = getNoonce();
	requestUrl = url;
	
	key = consumerSecret + "&";
	
	if ( signatureMethod == "HMAC-SHA1") {
		signatureBaseString = getSignatureBaseString( getBaseString() );
		signature = format::encrypt(signatureBaseString.c_str(), key.c_str());
	} else {
		signature = key;
	}
	
	basicHeader.push_back("oauth_callback=\"" + format::encode( callBack.c_str(), curl ) + "\", ");
	basicHeader.push_back("oauth_consumer_key=\"" + format::encode( consumerKey.c_str(), curl ) +"\", ");
	basicHeader.push_back("oauth_nonce=\"" + format::encode( noonce.c_str(), curl ) + "\", ");
	basicHeader.push_back("oauth_signature=\"" + format::encode( signature.c_str(), curl ) + "\", ");
	basicHeader.push_back("oauth_signature_method=\"" + format::encode( signatureMethod.c_str(), curl ) + "\", ");
	basicHeader.push_back("oauth_timestamp=\"" + format::encode( timeStamp.c_str(), curl ) + "\", ");
	basicHeader.push_back("oauth_version=\"" + format::encode( oauthVersion.c_str(), curl ) + "\"");
	
	postData = format::vectorToString(basicHeader);
	headerData = "Authorization: OAuth " + postData;
	headers = curl_slist_append(headers, headerData.c_str() );
	headers = curl_slist_append(headers, "Connection: Keep-Alive" );
	headers = curl_slist_append(headers, "Content-Type: text/html; charset=utf-8" );
	postData = format::replaceInString(postData, ", ", "&");
	
	std::cout << std::endl << "header data: " << headerData << std::endl << std::endl;
	std::cout << "post data: " << postData << std::endl << std::endl;
	std::cout << "request url: " << requestUrl << std::endl << std::endl << std::endl;
	
	postReturn = post(postData, headers, requestUrl);
	
	if ( ( postReturn.find("oauth_token=") != std::string::npos ) && ( postReturn.find("&oauth_token_secret=") != std::string::npos ) ) {
		return postReturn;
	} else {
		std::cout << "authentication failed access token request" << std::endl;
		return 0;
	}
}

std::string Oauth::requestAccessToken(std::string token, std::string tokenString, std::string pinCode , std::string url) {
	std::vector <std::string> basicHeader;
	struct curl_slist *headers=NULL;
	std::string key;
	std::string postData;
	std::string headerData;
	std::string signatureBaseString;
	std::string signature;
	std::string postReturn;
	
	timeStamp = getTime();
	noonce = getNoonce();
	requestUrl = url+"?oauth_token="+token;
	
	key = consumerSecret + "&" + tokenString;
	
	if ( signatureMethod == "HMAC-SHA1") {
		signatureBaseString = getSignatureBaseString( getBaseString( token ) );
		signature = format::encrypt(signatureBaseString.c_str(), key.c_str());
	} else {
		signature = key;
	}
	
	basicHeader.push_back("oauth_consumer_key=\"" + format::encode( consumerKey.c_str(), curl ) +"\", ");
	basicHeader.push_back("oauth_nonce=\"" + format::encode( noonce.c_str(), curl ) + "\", ");
	basicHeader.push_back("oauth_signature=\"" + format::encode( signature.c_str(), curl ) + "\", ");
	basicHeader.push_back("oauth_signature_method=\"" + format::encode( signatureMethod.c_str(), curl ) + "\", ");
	basicHeader.push_back("oauth_timestamp=\"" + format::encode( timeStamp.c_str(), curl ) + "\", ");
	basicHeader.push_back("oauth_token=\"" + format::encode( token.c_str(), curl ) + "\", ");
	basicHeader.push_back("oauth_verifier=\"" + format::encode( pinCode.c_str(), curl ) + "\", ");
	basicHeader.push_back("oauth_version=\"" + format::encode( oauthVersion.c_str(), curl ) + "\"");
	
	postData = format::vectorToString(basicHeader);
	headerData = "Authorization: OAuth " + postData;
	headers = curl_slist_append(headers, headerData.c_str() );
	headers = curl_slist_append(headers, "Connection: Keep-Alive" );
	headers = curl_slist_append(headers, "Content-Type: text/html; charset=utf-8" );
	postData = format::replaceInString(postData, ", ", "&");
	
	std::cout << std::endl << "header data: " << headerData << std::endl << std::endl;
	std::cout << "post data: " << postData << std::endl << std::endl;
	std::cout << "request url: " << requestUrl << std::endl << std::endl << std::endl;
	
	//httpMethod = "GET";
	postReturn = post(postData, headers, requestUrl);
	std::cout << "restricted data permission: " << postReturn << std::endl;
	
	return postReturn;
	/*
	//verify call back function 
	if ( ( postReturn.find("oauth_token=") != std::string::npos ) && ( postReturn.find("&oauth_token_secret=") != std::string::npos ) ) {
		return postReturn;
	} else {
		std::cout << "authentication failed" << std::endl;
		return 0;
	}
	*/
}

std::string Oauth::post(std::string postData, struct curl_slist *headers, std::string url) {
	CURLcode curlResult;
	std::string curlapiPageBuffer;
	std::string curlHeaderBuffer;
	
	///*debug and verbose instructions*/
	/*
	struct data config;
	config.trace_ascii = 1; //enable ascii tracing
	curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, format::my_trace);
	curl_easy_setopt(curl, CURLOPT_DEBUGDATA, &config);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); //the DEBUGFUNCTION has no effect until we enable VERBOSE
	*/
	
	///*callback function instructions*/
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, format::writer);
	curl_easy_setopt(curl, CURLOPT_WRITEHEADER, &curlHeaderBuffer); //shows response headers
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlapiPageBuffer);
	
	///*url and header instructions*/
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str() );
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	
	///*ssl instructions*/
	//curl_easy_setopt(curl, CURLOPT_CAINFO, "./VeriSignClass3ExtendedValidationSSLCA");
	//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	
	if (httpMethod == "GET") {
		///*get request*/
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
	} else if (httpMethod == "POST") {
		///*post request*/
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
	}
	
	///*send the request*/
	curlResult = curl_easy_perform(curl);
	
	///*clean up*/	
	curl_slist_free_all(headers); //free the header list
	//curl_easy_cleanup(curl); //clean up
	
	std::cout << curlHeaderBuffer << std::endl;
	std::cout << curlapiPageBuffer << std::endl;
	
	// Did we succeed?
	if (curlResult == CURLE_OK) {
		//std::cout << std::endl << "SUCCESS! message sent to Twitter!" << std::endl; //unix style coding requires no confirmation for expected behaviours
		return curlapiPageBuffer;
	} else {
		std::cout << std::endl << "OUPS! there was an error. The message was not sent to URL." << std::endl;
		return 0;
	}
}
