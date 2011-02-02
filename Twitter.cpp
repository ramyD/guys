#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "Twitter.h"

Twitter::Twitter() {
	bitly = new Bitly;
	myOauth = new Oauth("Oe8TiUN5SlfY2mRGKiRlxw", 
						"fA4iyWCQL5oR2ArYNYHmJ7THZH2BOmdHGEv6XsUOKI", 
						"oob", 
						"HMAC-SHA1");
	return;
}

// This is the writer call back function used by curl
int Twitter::twitterWriter(char *data, size_t size, size_t nmemb, std::string *buffer) {
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


std::string Twitter::parsePage(std::string token, std::string twitterURL, std::string *resultUrl) {
	CURL *curl;
	curl = curl_easy_init();
	CURLcode curlResult;
	std::string twitterPageBuffer;
	std::string url = twitterURL + "?oauth_token=" + token;
	*resultUrl = url;
	
	//std::cout << "<<< parsing twitter page: " << url << " with get request" << std::endl;
	
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, format::writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &twitterPageBuffer);
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str() );
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
	curlResult = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	
	return twitterPageBuffer;
}


std::string Twitter::getTwitterFormAction(std::string *twitterLoginPage) {
	//std::cout << "<<< getting twitter form action" << std::endl;
	std::string tagForm = "<form action=\"";
	size_t tagBegin, tagEnd;
	tagBegin = twitterLoginPage->find(tagForm);
	tagBegin += tagForm.length(); //puts the tag after the first "
	tagEnd = twitterLoginPage->find_first_of("\"", tagBegin+1); //finds the end "
	
	//at this point, tagBegin and tagEnd have the inclusive position of the string we want, always counting from the begining.
	
	return twitterLoginPage->substr(tagBegin, tagEnd - tagBegin); //substring take the first parameter as a position (count from 0) and a length (count from 1) which requires substraction
}


std::string Twitter::getTwitterAuthenticityToken(std::string *twitterLoginPage) {
	//std::cout << "<<< getting twitter authenticity token" << std::endl;
	std::string tagInput = "<input name=\"authenticity_token\" type=\"hidden\" value=\"";
	size_t tagBegin, tagEnd;
	tagBegin = twitterLoginPage->find(tagInput);
	tagBegin += tagInput.length(); //puts the tag after the first "
	tagEnd = twitterLoginPage->find_first_of("\"", tagBegin+1); //finds the end "
	
	//at this point, tagBegin and tagEnd have the inclusive position of the string we want, always counting from the begining.
	
	return twitterLoginPage->substr(tagBegin, tagEnd - tagBegin); //substring take the first parameter as a position (count from 0) and a length (count from 1) which requires substraction;
}


std::string Twitter::getPassword() {
	char passwordChar;
	std::stringstream passwordStream;
	std::string password;
	
	std::cout << "please enter your twitter password: ";
	do {
		passwordChar = getchar();
		passwordStream << passwordChar;
		putchar ('*');
	} while (passwordChar != '\n'); //lol this doesn't work
	
	password = passwordStream.str();
	password.erase(password.end()-1 ); //remove the last character which is "enter"
	
	//std::cout << "password debug: " << password << " password length: " << password.length() << std::endl;
	
	return passwordStream.str();
}


std::string Twitter::getCode(std::string postData, std::string postUrl, std::string refererUrl) {
	//std::cout << "<<< getting pin code from " << postUrl << std::endl;
	CURL *curl;
	curl = curl_easy_init();
	CURLcode curlResult;
	std::string twitterPageBuffer;
	char *referedUrl = const_cast<char *>( refererUrl.c_str() );
	std::string url = postUrl;
	//std::cout << "<<< referer URL is: " << referedUrl << std::endl;

	char postCharData[256];
	memset(postCharData, '\0', 256); //this may cause problems if email or username is too long
	strncpy( postCharData, postData.c_str(), postData.length()-1 );
	postCharData[postData.length()-1] = '\0';
	postCharData[postData.length()] = '\0';
	//std::cout << "<<< post character data is: " << postCharData << std::endl;

/*
	///debug and verbose instructions
	struct data config;
	config.trace_ascii = 1; //enable ascii tracing
	curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, format::my_trace);
	curl_easy_setopt(curl, CURLOPT_DEBUGDATA, &config);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); //the DEBUGFUNCTION has no effect until we enable VERBOSE
*/	

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, format::writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &twitterPageBuffer);
	curl_easy_setopt(curl, CURLOPT_REFERER, referedUrl );
	curl_easy_setopt(curl, CURLOPT_URL, postUrl.c_str() );
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	//curl_easy_setopt(curl, CURLOPT_HEADER, 0);
	//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str() );
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postCharData );
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curlResult = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	
	return twitterPageBuffer;
}


std::string Twitter::getPinNumber(std::string *twitterCodePage) {
	std::cout << "<<< parsing for twitter pin" << std::endl;
	std::string tagInput = "<div id=\"oauth_pin\">";
	size_t tagBegin, tagEnd;
	tagBegin = twitterCodePage->find(tagInput);
	tagBegin += tagInput.length(); //puts the tag after the first "
	tagEnd = twitterCodePage->find_first_of("</div>", tagBegin+1); //finds the end "
	
	//at this point, tagBegin and tagEnd have the inclusive position of the string we want, always counting from the begining.
	
	return twitterCodePage->substr(tagBegin, tagEnd - tagBegin); //substring take the first parameter as a position (count from 0) and a length (count from 1) which requires substraction;
}


std::string Twitter::cleanPinResult(std::string pinNumber) {
	std::string numStorage;
	std::string::iterator it;

	for (it=pinNumber.begin(); it < pinNumber.end(); ++it) {
		if( *it >= 48 && *it <= 57) numStorage += *it;
	}

	return numStorage;
}


std::string Twitter::hackTwitter(std::string token) {
	std::string twitterLoginPage;
	std::string formAction;
	std::string authenticityToken;
	std::string password;
	std::string twitterPinPage;
	std::string twitterPinNumber;
	std::string twitterAuthorizationUrl;
	
	std::string postUrl = "https://api.twitter.com/oauth/authorize";
	std::string postData;
	std::string postDataHeader;
	std::stringstream postDataLength;
	std::string postAuthenticityToken, postOauthToken, postSessionEmailUser, postSessionPassword;
	postAuthenticityToken = "authenticity_token";
	postOauthToken = "oauth_token";
	postSessionEmailUser = "session%5Busername_or_email%5D";
	postSessionPassword = "session%5Bpassword%5D";
	
	twitterLoginPage = parsePage(token, postUrl, &twitterAuthorizationUrl);
	
	//std::cout << "twitter login page %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl << twitterLoginPage << std::endl;
	
	formAction = getTwitterFormAction(&twitterLoginPage);
	
	//std::cout << "form action is: " << formAction << std::endl;
	
	authenticityToken = getTwitterAuthenticityToken(&twitterLoginPage);
	password = getPassword();
	
	postData = postAuthenticityToken + '=' + authenticityToken + '&' + postOauthToken + '=' + token + '&' + postSessionEmailUser + '=' + UserName + '&' + postSessionPassword + '=' + password;

	//std::cout << "<<< post Data length: " << postData.length() << std::endl;
	//std::cout << "post data 2 length is : " << strlen(postData.c_str() ) << std::endl;	

	//postDataHeader = "Content-Type: application/x-www-form-urlencode\nContent-Length: ";
	//postDataLength << postData.length() -1;

	//postData = postDataHeader + postDataLength.str() + "\n\n" + postData;
	
	//std::cout << "post data is: " << postData << std::endl;


	twitterPinPage = getCode(postData, formAction, twitterAuthorizationUrl);
	twitterPinNumber = getPinNumber(&twitterPinPage);
	twitterPinNumber = cleanPinResult(twitterPinNumber);

	return twitterPinNumber;
}


void Twitter::promptUsername() {
	std::string filePath = configPath + "twitter";
	std::cout << "Enter your twitter username or email: ";
	getline (std::cin, UserName);
	
	std::ofstream myfile;
	myfile.open (filePath.c_str() );
	myfile << UserName;
	myfile.close();
	
	return;
}


void Twitter::generateAccessToken() {
	std::string requestTokenString;
	std::string accessTokenString;
	std::string token;
	std::string tokenSecret;
	std::string pinCode;
	
	requestTokenString = myOauth->requestRequestToken("https://api.twitter.com/oauth/request_token", "POST");
	token = format::extract("oauth_token", '&', requestTokenString); //this method should pass by reference, not by value
	tokenSecret = format::extract("oauth_token_secret", '&', requestTokenString);
	
	pinCode = hackTwitter(token);
	
	accessTokenString = myOauth->requestAccessToken(token, tokenSecret, pinCode, "https://api.twitter.com/oauth/access_token", "POST");
	
	token = format::extract("oauth_token", '&', accessTokenString);
	tokenSecret = format::extract("oauth_token_secret", '&', accessTokenString);
	
	std::string line;
	std::ifstream myifile;
	std::string filePath = configPath + "twitter";
	
	myifile.open(filePath.c_str() );
	if (myifile.is_open()) {
		while (! myifile.eof()) {
			getline (myifile,line);
		}
		myifile.close();
	}
	
	std::ofstream myofile;
	myofile.open (filePath.c_str());
	
	myofile << line << "\n" << token << "\n" << tokenSecret;
	myofile.close();
	
	return;
}


void Twitter::writeFile() {
	std::string line[3];
	std::ifstream myfile;
	std::string filePath = configPath + "twitter";
	
	myfile.open(filePath.c_str() );
	if (myfile.is_open()) {
		int i = 0;
		while (! myfile.eof() ) {
			getline (myfile,line[i]);
			i++;
		}
		myfile.close();
		
		if (line[0] == "") {
			//std::cout << "file has no username" << std::endl;
			promptUsername();
		}
		
		if (line[1] == "" || line[2] == "") {
			//std::cout << "file has no access token" << std::endl;
			generateAccessToken();
		}
		
	} else std::cout << "Unable to open file" << std::endl;
	
	return;
}


void Twitter::readFile(std::string *token, std::string *tokenSecret) {
	std::string line[3];
	std::ifstream myfile;
	std::string filePath = configPath + "twitter";
	
	myfile.open(filePath.c_str() );
	if (myfile.is_open()) {
		int i = 0;
		while (! myfile.eof() ) {
			getline (myfile,line[i]);
			i++;
		}
		myfile.close();
	} else std::cout << "Unable to open file" << std::endl;
	
	if (line[0] != "" && line[1] != "" && line[2] != "") {
		UserName =  line[0];
		*token = line[1];
		*tokenSecret = line[2];
		//std::cout << "UserName: " << UserName << " token: " << &token << " tokenSecret: " << &tokenSecret << std::endl;
	}
}


void Twitter::post(std::string message, std::string configFilePath){

	configPath = configFilePath; //move to constructor?

	if (message.size() <= 0) { //if there is no quick post
		std::cout << "**your Post: ";
		getline (std::cin, message);
	} else { //if there is a quick post
		std::cout << "**your Post: " << message << std::endl;
	}
	
	message = bitly->checkForUrl(message);
	
	if (message.length() > 140){
		if ( (message.length() - 140) < 2){
			std::cout << "OUPS! message is 1 character above the 140 character limit" << std::endl;
		} else {
			std::cout << "OUPS! message is "<< message.length() - 140 << " characters above the limit" << std::endl;
		}
	} else {
		std::string token;
		std::string tokenSecret;
		std::string resourceTokenString;
		
		writeFile();
		readFile(&token, &tokenSecret);
		
		resourceTokenString = myOauth->requestResourceToken(token, tokenSecret, "http://api.twitter.com/1/statuses/update.json", "POST","status=", message);
	}
	
	return;
}


const char * Twitter::getIndent( unsigned int numIndents ) { // a utility function defining a very simple method to indent a line of text
	static const char * pINDENT = "                                      + ";
	static const unsigned int LENGTH = strlen( pINDENT );

	if ( numIndents > LENGTH ) numIndents = LENGTH;

	return &pINDENT[ LENGTH-numIndents ];
}


void Twitter::dump_to_stdout( TiXmlNode * pParent, unsigned int indent){
	if ( !pParent ) return;

	TiXmlText *pText;
	int t = pParent->Type();
	printf( "%s", getIndent( indent));

	switch ( t ) {
	case TiXmlNode::DOCUMENT:
		printf( "Document" );
		break;

	case TiXmlNode::ELEMENT:
		printf( "Element \"%s\"", pParent->Value() );
		break;

	case TiXmlNode::COMMENT:
		printf( "Comment: \"%s\"", pParent->Value());
		break;

	case TiXmlNode::UNKNOWN:
		printf( "Unknown" );
		break;

	case TiXmlNode::TEXT:
		pText = pParent->ToText();
		printf( "Text: [%s]", pText->Value() );
		break;

	case TiXmlNode::DECLARATION:
		printf( "Declaration" );
		break;
	default:
		break;
	}

	printf( "\n" );

	TiXmlNode * pChild;

	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) {
		dump_to_stdout( pChild, indent+2 );
	}
}


void Twitter::getTimeline(std::string configFilePath){
	configPath = configFilePath; //move to constructor?
	
	std::string token;
	std::string tokenSecret;
	std::string resourceTokenString;

	writeFile();
	readFile(&token, &tokenSecret);

	resourceTokenString = myOauth->requestResourceToken(token, tokenSecret, "http://api.twitter.com/1/statuses/friends_timeline.xml", "GET");


	TiXmlDocument doc("friends_timeline.xml");
	bool loadOkay = doc.Parse(resourceTokenString.c_str(), 0, TIXML_ENCODING_UTF8);

	if (!loadOkay) {
		printf( "Could not load timeline xml. Error='%s'. Exiting.\n", doc.ErrorDesc() );
		exit( 1 );
	}

	if (loadOkay) {
		//dump_to_stdout( &doc ); //this dumps the entire xml using tinyxml's structure form (very sexy)
		
		TiXmlHandle docHandle(&doc);
		
		char blue[] = { 0x1b, '[', '1', ';', '3', '4', 'm', 0 };
		char normal[] = { 0x1b, '[', '0', ';', '3', '9', 'm', 0 };

		for (int i=19; i>=0; i--){
			TiXmlElement *created_at = docHandle.FirstChild( "statuses" ).Child( "status", i ).Child( "created_at", 0 ).Element();
			TiXmlElement *text = docHandle.FirstChild( "statuses" ).Child( "status", i ).Child( "text", 0 ).Element();
			TiXmlElement *screen_name = docHandle.FirstChild( "statuses" ).Child( "status", i ).FirstChild( "user" ).Child( "screen_name", 0 ).Element();
			
			if (created_at && text && screen_name){
				std::cout << blue << screen_name->GetText() << normal << ": " << text->GetText() << std::endl << "tweeted on: " << created_at->GetText() << std::endl << std::endl;
			}
		}
	}
}
