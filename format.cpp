/*
 *      format.cpp
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

#include "format.h"
#include "data.h"

std::string format::encode(std::string stringToEncode, CURL *curl) {
	std::ostringstream temporaryStream;
	std::string encodedCharacter;
	for (unsigned int i=0; i<stringToEncode.size(); i++) {
		if ( isCharacterReserved(stringToEncode[i]) ) {
			encodedCharacter = curl_easy_escape(curl, &stringToEncode[i], 0 );
			temporaryStream << encodedCharacter[0] << encodedCharacter[1] << encodedCharacter[2];
		} else {
			temporaryStream << stringToEncode[i];
		}
	}
	return temporaryStream.str();
}

bool format::isCharacterReserved(char characterToEvaluate) {
	//these are all the unreserved characters that we don't need to decode
	char unreservedCharacters[] = {'a','A','b','B','c','C','d','D','e','E','f','F','g','G','h','H','i','I','j','J','k','K','l','L','m','M','n','N','o','O','p','P','q','Q','r','R','s','S','t','T','u','U','v','V','w','W','x','X','y','Y','z','Z','0','1','2','3','4','5','6','7','8','9','-','.','_','~'};
	std::set<char> unreservedCharSet(unreservedCharacters, unreservedCharacters + sizeof(unreservedCharacters) );
	bool isReserved = false; //by default, we assume the character is part of the unreserved set
	if ( unreservedCharSet.find(characterToEvaluate) == unreservedCharSet.end() ) {
		isReserved = true;
	}
	return isReserved;
}

std::vector <std::string> format::sortVector(std::vector <std::string> vectorToSort, char tailCharacter) {
	sort(vectorToSort.begin(), vectorToSort.end() ); //sort the vector elements alphabetically
	if ( vectorToSort.back().at(vectorToSort.back().length()-1) == tailCharacter) { //check if the last element matches the tail character and remove it. this is sometimes a comma or an ampersand
		std::string lastString = vectorToSort.back(); //assign the last vector to a string
		vectorToSort.pop_back(); //remove the last element in the vector
		lastString = lastString.substr(0,lastString.length()-1); //grab all but the last character
		vectorToSort.push_back(lastString); //put the modified string back into the vector
	}
	return vectorToSort;
}

std::string format::vectorToString(std::vector <std::string> stringToPrint) {
	std::ostringstream temporaryStream;
	for (unsigned int i=0; i < stringToPrint.size(); i++) {
		temporaryStream << stringToPrint[i];
	}
	return temporaryStream.str();
}

std::string format::encrypt(std::string inputText, std::string inputKey) {
	//inputText = "POST&http%3A%2F%2Fapi.twitter.com%2F1%2Fstatuses%2Fupdate.json&oauth_consumer_key%3DOe8TiUN5SlfY2mRGKiRlxw%26oauth_nonce%3D70DCbXcB40bdQ8nKOojLDGE8tc8ej%26oauth_signature_method%3DHMAC-SHA1%26oauth_timestamp%3D1282428082%26oauth_token%3D25654146-M6Pnr35Ju6F1u34W2RoUITEnJTO0eM4XOnSI6Ng4d%26oauth_version%3D1.0%26status%3Dtest";

	//inputKey = "A4iyWCQL5oR2ArYNYHmJ7THZH2BOmdHGEv6XsUOKI&q12345678901234567890123";

	byte *key = (byte*)inputKey.c_str(); //initializing key
	byte digest [ CryptoPP::HMAC<CryptoPP::SHA1>::DIGESTSIZE ]; //initlizing digest length

	CryptoPP::HMAC<CryptoPP::SHA1> hmac( key, inputKey.size() ); //use the key
	hmac.Update( (byte*)inputText.c_str(), inputText.size() ); //use the text
	hmac.Final( digest ); //fill the digest with the encrypted values

	CryptoPP::Base64Encoder base64; 
	base64.Put(digest, sizeof(digest) ); //use the digest to base64 encode
	base64.MessageEnd();
	base64.MessageSeriesEnd();

	unsigned int size = calculateBase64Size(sizeof(digest) ); //calculate the size of the base64 encoded digest!
	byte* encodedValues = new byte[size]; //giving enough room for the encoded values array
	char transferValues[size];

	base64.Get( encodedValues, size ); //filling the variable with the encoded values
	std::string encodedString;

	memcpy(transferValues, encodedValues, size);

	for (unsigned int i=0; i<size; i++) {
		encodedString += transferValues[i];
	}
	
	return encodedString;
}

int format::calculateBase64Size(int digestSize) {
	int newSize;
	newSize = (digestSize + 2 - ((digestSize + 2) % 3)) * 4 / 3;
	return newSize;
}

std::string format::replaceInString(std::string stringToSwap, const std::string find, std::string replace) {
	size_t j;
	for ( ; (j = stringToSwap.find( find )) != std::string::npos ; ) {
		stringToSwap.replace( j, find.length(), replace );
	}
	return stringToSwap;
}

std::string format::extract(std::string parameter, char delimitingCharacter, std::string fullString) {
	unsigned int parameterPosition;
	unsigned int valuePositionBegin;
	unsigned int valuePositionEnd;
	
	parameterPosition = fullString.find(parameter);
	
	if (parameterPosition == std::string::npos) {
		std::cout << "parameter not found" << std::endl;
		return 0;
	}
	
	parameterPosition += parameter.size(); //get a location right at the end of the parameter, 
    //including +1 for the parameter divider, usually a = <------ too specific
	valuePositionBegin = parameterPosition + 1; //add 1 for first in array vs first in sequence
    
	valuePositionEnd = fullString.find_first_of(delimitingCharacter, valuePositionBegin);
	
	if (valuePositionEnd == std::string::npos) valuePositionEnd = fullString.size(); //if the delimeter is not found, assume it is the last parameter/value pair
	
	return fullString.substr(valuePositionBegin, valuePositionEnd-valuePositionBegin);
}

int format::my_trace(CURL *handle, curl_infotype type, unsigned char *data, size_t size, void *userp) {
  struct data *config = (struct data *)userp;
  const char *text;
  (void)handle; /* prevent compiler warning */

  switch (type) {
  case CURLINFO_TEXT:
    fprintf(stderr, "== Info: %s", data);
  default: /* in case a new one is introduced to shock us */
    return 0;

  case CURLINFO_HEADER_OUT:
    text = "=> Send header";
    break;
  case CURLINFO_DATA_OUT:
    text = "=> Send data";
    break;
  case CURLINFO_HEADER_IN:
    text = "<= Recv header";
    break;
  case CURLINFO_DATA_IN:
    text = "<= Recv data";
    break;
  case CURLINFO_SSL_DATA_IN:
    text = "<= Recv SSL data";
    break;
  case CURLINFO_SSL_DATA_OUT:
    text = "<= Send SSL data";
    break;
  }

  format::dump(text, stderr, data, size, config->trace_ascii);
  return 0;
}

void format::dump(const char *text, FILE *stream, unsigned char *ptr, size_t size, char nohex) {
  size_t i;
  size_t c;

  unsigned int width=0x10;

  if(nohex)
    /* without the hex output, we can fit more on screen */
    width = 0x40;

  fprintf(stream, "%s, %zd bytes (0x%zx)\n", text, size, size);

  for(i=0; i<size; i+= width) {

    fprintf(stream, "%04zx: ", i);

    if(!nohex) {
      /* hex not disabled, show it */
      for(c = 0; c < width; c++)
        if(i+c < size)
          fprintf(stream, "%02x ", ptr[i+c]);
        else
          fputs("   ", stream);
    }

    for(c = 0; (c < width) && (i+c < size); c++) {
      /* check for 0D0A; if found, skip past and start a new line of output */
      if (nohex && (i+c+1 < size) && ptr[i+c]==0x0D && ptr[i+c+1]==0x0A) {
        i+=(c+2-width);
        break;
      }
      fprintf(stream, "%c",
              (ptr[i+c]>=0x20) && (ptr[i+c]<0x80)?ptr[i+c]:'.');
      /* check again for 0D0A, to avoid an extra \n if it's at width */
      if (nohex && (i+c+2 < size) && ptr[i+c+1]==0x0D && ptr[i+c+2]==0x0A) {
        i+=(c+3-width);
        break;
      }
    }
    fputc('\n', stream); /* newline */
  }
  fflush(stream);
}

int format::writer(char *data, size_t size, size_t nmemb, std::string *buffer) { //This is the writer call back function used by curl
	int result = 0; //What we will return
	if (buffer != NULL){ //Is there anything in the buffer?
		buffer->append(data, size * nmemb); //Append the data to the buffer
		result = size * nmemb; //How much did we write?
	}
	return result;
}
