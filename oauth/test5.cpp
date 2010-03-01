//      test5.cpp
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

///g++ -Wall -std=c++98 -pedantic -Os test5.cpp -o test5 -lcurl -lcrypto++
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include <crypto++/cryptlib.h>
#include <crypto++/sha.h>
#include <crypto++/hmac.h>
#include <crypto++/base64.h>
#include <curl/curl.h>
#include <uudeview.h>

using namespace std;

string httpMethod;
string requestTokenUrl;

string realm;
string consumerKey;
string signatureMethod;
string timeStamp;
string noonce;
string oauthVersion;
string callBack;

string token;
string file;
string fileSize;

CURL *curl;

string getTimeInSeconds() {
    time_t seconds;
    ostringstream temporaryStream;
    seconds = time (NULL);
    temporaryStream << seconds;
    return temporaryStream.str();
}

string makeNoonce() {
    ostringstream temporaryStream;
    const int noonceLength = 16;
    srand((unsigned) time(NULL)); //gives a seed based on time for the random function
    for (int i = 0; i < noonceLength; i++) {
        temporaryStream << (char) (rand() % 26 + 'a');
    }
    return temporaryStream.str();
}

void initializeGlobalVariables() {
    httpMethod = "GET";
    requestTokenUrl = "http://twitter.com/oauth/request_token";
    realm = "http://twitter.com/";
    consumerKey = "3LzLFcBF2HKerm3DFb05EQ";
    signatureMethod = "HMAC-SHA1";
    timeStamp = getTimeInSeconds();
    noonce = makeNoonce();
    oauthVersion = "1.0";
    callBack = "oob";
    
    curl = curl_easy_init();
}

string printVector(vector <string> stringToPrint) {
    ostringstream temporaryStream;
    for (unsigned int i=0; i < stringToPrint.size(); i++) {
        temporaryStream << stringToPrint[i];
    }
    return temporaryStream.str();
}

string replaceInString(string &stringToSwap, const string find, string replace) {
    size_t j;
    for ( ; (j = stringToSwap.find( find )) != string::npos ; ) {
        stringToSwap.replace( j, find.length(), replace );
    }
    return stringToSwap;
}

string urlEncode(string &stringToEncode) {
    stringToEncode = curl_easy_escape(curl, stringToEncode.c_str(), 0 );
    return stringToEncode;
}

vector <string> getBasicHeader(string signature = "0") {
    vector <string> basicHeader;
    basicHeader.push_back("Authorization: ");
    basicHeader.push_back("OAuth realm=" + realm + ",");
    basicHeader.push_back("oauth_consumer_key=" + consumerKey +",");
    basicHeader.push_back("oauth_signature_method=" + signatureMethod + ",");
    if (signature != "0") basicHeader.push_back("oauth_signature=" + signature + ",");
    basicHeader.push_back("oauth_timestamp=" + timeStamp + ",");
    basicHeader.push_back("oauth_nonce=" + noonce + ",");
    basicHeader.push_back("oauth_callback=" + callBack + ",");
    basicHeader.push_back("oauth_version=" + oauthVersion); //this is, alphabetically, the last one, so it has no comma
    
    return basicHeader;
}

string getBaseString() {
    vector <string> header;
    string printedHeader;
    string baseString;
    string requestUrl;
    
    header = getBasicHeader();
    header.erase(header.begin(), header.begin()+2); //erase first 2 elements so we get only the parameters 
    sort(header.begin(), header.end() ); //sorting the remaining elements
    for (unsigned int i=0; i<header.size(); i++) { //concatenating base string elements
        replaceInString(header[i], ",", "&");
    }
    printedHeader = printVector(header);
    requestUrl = requestTokenUrl;
    urlEncode(printedHeader); //encoding parameters
    urlEncode(requestUrl); //encoding parameters
    replaceInString(requestUrl, "%2E", "."); //i guess we aren't supposed to encode periods?
    replaceInString(printedHeader, "%2E", ".");
    replaceInString(printedHeader, "%5F", "_");
    replaceInString(printedHeader, "%2D", "-");
    baseString = httpMethod + "&" + requestUrl + "&" + printedHeader; //putting the base string together

    return baseString;
}

int calculateBase64Size(int digestSize) {
    int newSize;
    newSize = (digestSize + 2 - ((digestSize + 2) % 3)) * 4 / 3;
    return newSize;
}

string encryptAndBase64(string inputKey, string inputText) {
    //inputKey = "kd94hf93k423kf44&pfkkdhi9sl3r4s00";
    //inputKey = "owBYqITppRzX4vfi920bvLpZcUUvJP2P8TCJ4DHvJz4&";
    //inputText = "GET&http%3A%2F%2Fphotos.example.net%2Fphotos&file%3Dvacation.jpg%26oauth_consumer_key%3Ddpf43f3p2l4k3l03%26oauth_nonce%3Dkllo9940pd9333jh%26oauth_signature_method%3DHMAC-SHA1%26oauth_timestamp%3D1191242096%26oauth_token%3Dnnch734d00sl2jdk%26oauth_version%3D1.0%26size%3Doriginal";

    byte *key = (byte*)inputKey.c_str(); //initializing key
    byte digest [ CryptoPP::HMAC<CryptoPP::SHA1>::DIGESTSIZE ]; //initlizing digest length

    CryptoPP::HMAC<CryptoPP::SHA1> hmac( key, inputKey.size()+1 ); //use the key
    hmac.Update( (byte*)inputText.c_str(), inputText.size() ); //use the text
    hmac.Final( digest ); //fill the digest with the encrypted values
    
    CryptoPP::Base64Encoder base64; 
    base64.Put(digest, sizeof(digest) ); //use the digest to base64 encode
    base64.MessageEnd();
    base64.MessageSeriesEnd();
    
    unsigned int size = calculateBase64Size(sizeof(digest) ); //calculate the size of the base64 encoded digest!
    byte* encodedValues = new byte[size]; //giving enough room for the encoded values array
    
    base64.Get( encodedValues, size ); //filling the variable with the encoded values
    string encodedCharacter = (char*)encodedValues;
    encodedCharacter.at(size) = '\0'; //giving the string a proper \0 ending

    return encodedCharacter;
}

string getOauthSignature(string stringToSign) {
    string consumerSecret;
    string hmacKey;
    string oauthSignature;
    
    consumerSecret = "owBYqITppRzX4vfi920bvLpZcUUvJP2P8TCJ4DHvJz4&";
    //consumerSecret = "kd94hf93k423kf44&pfkkdhi9sl3r4s00";
    hmacKey = consumerSecret; //appending an ampersand even though no token is present
    //urlEncode(hmacKey); //encoding
    oauthSignature = encryptAndBase64(hmacKey, stringToSign);

    return oauthSignature;
}

string getCurlHeader() {
    vector <string> header;
    string baseString;
    string signature;
    string headerString;
    
    baseString = getBaseString();
    signature = getOauthSignature(baseString);
    
    cout << endl << "signature " << signature << endl;
    
    header = getBasicHeader(signature);
    header.erase(header.begin(), header.begin()+2); //erase first 2 elements so we get only the parameters 
    sort(header.begin(), header.end() ); //sorting all but the first 2 elements of thea header in alphabetical order
    headerString = printVector(header);
    replaceInString(headerString, ",", "&");//concatenating base string elements
    //urlEncode(headerString);
    
    return headerString;
}

static int writer(char *data, size_t size, size_t nmemb, std::string *buffer) { //This is the writer call back function used by curl
  int result = 0; //What we will return
  if (buffer != NULL){ //Is there anything in the buffer?
    buffer->append(data, size * nmemb); //Append the data to the buffer

    result = size * nmemb; //How much did we write?
  }
  return result;
}

int main(int argc, char** argv) {
    initializeGlobalVariables();
    CURLcode curlResult;
    string curlapiPageBuffer;
    string curlHeader;
    string curlCookie;
    string curlUrl;
    
    curlHeader = getCurlHeader();
    
    curlCookie = "/home/ramy/cpp/oauth/twitter/cookies.txt";
    curlUrl = "http://twitter.com/oauth/request_token";
    
    curlUrl = curlUrl + "?" + curlHeader;
    
    cout << "curl url: " << curlUrl << endl;
    
    //curl_easy_setopt(curl, CURLOPT_HEADER, curlHeader.c_str() );
    //curl_easy_setopt(curl, CURLOPT_HEADER, "Authenticate: OAuth realm=\"http://twitter.com/\"");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlapiPageBuffer);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    //curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_COOKIE, curlCookie.c_str() );
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, curlCookie.c_str() );
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, curlCookie.c_str() );
    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, curlHeader.c_str() );
    curl_easy_setopt(curl, CURLOPT_URL, curlUrl.c_str() );

    curlResult = curl_easy_perform(curl);

    cout << curlapiPageBuffer << endl;

    // Did we succeed?
    if (curlResult == CURLE_OK){
        //cout << "SUCCESS! message sent to Twitter!" << endl; //unix style coding requires no confirmation for expected behaviours
        return 1;
    } else{
        //cout << "OUPS! there was an error. The message was not sent to Twitter." << endl;
        return 0;
    }

    return 0;
}
