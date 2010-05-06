//      Bitly.cpp
//      
//      Copyright 2010 Alexis Morin <alexis@alexis-laptop>
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


#include "Bitly.h"

Bitly::Bitly(){
    userlogin = "guys";
    apiKey = "R_86305ef3a8a76de670cd9663b9c15637";
    
    if (userlogin.length() == 0 || apiKey.length() == 0){
        std::cout << "you can not use Bitly, no API data specified" << std::endl;
        return;
    }
    
    curl = curl_easy_init();
}

int Bitly::interpretArguments(int argc, char* argv[]){
    if (argc < 2) { //if no arguments
        std::cout << "usage: ./shortener http://www.google.com" << std::endl;
        return 0;
    }
    
    std::ostringstream unformattedArguments;
    std::vector <std::string> arguments;
    for (int i = 0; i < argc; i++){  //assign each argument to a string array alled "arguments"
		unformattedArguments << argv[i];
		arguments.push_back(unformattedArguments.str());
		unformattedArguments.str("");
	}
    
    if(arguments[1].length() > 0){
        shorten(arguments[1]);
    }
    
    return 0;
}

// This is the writer call back function used by curl
static int bitlyWriter(char *data, size_t size, size_t nmemb, std::string *buffer){
  // What we will return
  int result = 0;

  // Is there anything in the buffer?
  if (buffer != NULL){
    // Append the data to the buffer
    buffer->append(data, size * nmemb);

    // How much did we write?
    result = size * nmemb;
  }

  return result;
}

void Bitly::shorten(std::string uri){
    
    if(uri.length() == 0){
       //URI for shortening is empty
        std::cout << "URI is empty" << std::endl;
    }else{
        //make the full URL to pass to the API
        fullUrl = "http://api.bit.ly/v3/shorten?login="+userlogin+"&apiKey="+apiKey+"&format=xml&uri="+uri;
        
        //std::cout << "shortening: " << uri << std::endl;
        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, bitlyWriter);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &apiPageBuffer);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        //set the URL to go to
        curl_easy_setopt(curl, CURLOPT_URL, fullUrl.c_str() );
        
        //doesn't seem to be performing too hot
        bitlyResult = curl_easy_perform(curl);
        
        //prints out the full response XML
        //std::cout << apiPageBuffer << std::endl;
        
        TiXmlDocument doc("shorten");
        
        bool loadOkay = doc.Parse(apiPageBuffer.c_str(), 0, TIXML_ENCODING_UTF8);
        
        if (!loadOkay) {
            //std::cout << "umm...nope" << std::endl;
        }else{
            //std::cout << "so far, so good" << std::endl;
            
            TiXmlHandle docHandle(&doc);
            
            TiXmlElement *status_code = docHandle.FirstChild("response").Child("status_code", 0).Element();
        
           /* 
             * assume status will be 200 for now. 
             * @TODO: add error handling
           */
            int statusCheck = atoi(status_code->GetText());
            
            bool shortenIsValid = 0;
            if(200 == statusCheck ){
                std::cout << "status 200" << std::endl;
                shortenIsValid = 1;
            }else if(403 == statusCheck){
                std::cout << "RATE_LIMIT_EXCEEDED" << std::endl;
                //shortenIsValid = 0;
            }else if(500 == statusCheck){
                std::cout << "INVALID_URI or MISSING_ARG_LOGIN" << std::endl;
                //shortenIsValid = 0;
            }else if(503 == statusCheck){
                std::cout << "UNKNOWN_ERROR" << std::endl;
                //shortenIsValid = 0;
            }
            
            if(shortenIsValid){
                TiXmlElement *shortUrl = docHandle.FirstChild("response").Child("data", 0).Child("url",0).Element();
                std::cout << shortUrl->GetText() << std::endl;
                //return shortUrl->GetText();
            }else{
                std::cout << "encountered an error, returning original uri" << uri << std::endl;
                //return uri;
            }
        }
    }
    return;
}
