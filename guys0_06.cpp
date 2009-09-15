/*
 *      guys0_06.cpp
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
//compile line: 
///g++ -Wall -std=c++98 -pedantic -Os tinyxml.cpp tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp guys0_06.cpp -o guys -lboost_program_options -lcurl

//twitter user info:
///lines 78 and 79

//facebook user info:
///lines 279, 280 and 281

#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
using namespace std;

#include <curl/curl.h>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "tinyxml.h"

string message;
bool donePosting;

CURL *curl;

static int writer(char *data, size_t size, size_t nmemb, std::string *buffer){ //This is the writer call back function used by curl
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


class Twitter{
private:
	string 	URL;
	string	UserName;
	string	Password;
	string 	Account;
	string	prependPost;
	string	statusUpdate;
	string	apiPageBuffer;
	
	CURLcode twitterResult;
	
public:
	Twitter(){ //login
		UserName = "YOUR USER NAME";
		Password = "YOUR PASSWORD";
		Account = UserName + ':' + Password;
		
		if (UserName.length() == 0){
			cout << "you can not use Twitter, no login data specified" << endl;
			return;
		}
	}

	void postMessage(string message){
		prependPost = "status=";
		statusUpdate = prependPost + message;
		
		URL = "http://twitter.com/statuses/update.xml";
		
		if (message.length() > 140){
			if ( (message.length() - 140) < 2){
				cout << "OUPS! message is 1 character above the 140 character limit" << endl;
			} else {
				cout << "OUPS! message is "<< message.length() - 140 << " characters above the limit" << endl;
			}
			donePosting = false;
		} else {
			///Now set up all of the curl options
			curl_easy_setopt(curl, CURLOPT_USERPWD, Account.c_str() );
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &apiPageBuffer);
			curl_easy_setopt(curl, CURLOPT_POST, 1 );
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, statusUpdate.c_str() );
			
			curl_easy_setopt(curl, CURLOPT_URL, URL.c_str() );
			
			//cout << "sending..." << endl; //unix style coding requires no confirmation for expected behaviours
			twitterResult = curl_easy_perform(curl);

			donePosting = true;

			// Did we succeed?
			if (twitterResult == CURLE_OK){
				//cout << "SUCCESS! message sent to Twitter!" << endl; //unix style coding requires no confirmation for expected behaviours
				return;
			} else{
				cout << "OUPS! there was an error. The message was not sent to Twitter." << endl;
				return;
			}
		}
	}


	const char * getIndent( unsigned int numIndents ) { // a utility function defining a very simple method to indent a line of text
		static const char * pINDENT = "                                      + ";
		static const unsigned int LENGTH = strlen( pINDENT );

		if ( numIndents > LENGTH ) numIndents = LENGTH;

		return &pINDENT[ LENGTH-numIndents ];
	}


	void dump_to_stdout( TiXmlNode * pParent, unsigned int indent = 0 ){
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

	
	void getTimeline(void){
		URL = "http://twitter.com/statuses/friends_timeline.xml";

		// Now set up all of the curl options
		curl_easy_setopt(curl, CURLOPT_USERPWD, Account.c_str() );
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &apiPageBuffer);
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1 );
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		
		curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
		
		//cout << "sending..." << endl; //unix style coding requires no confirmation for expected behaviours
		twitterResult = curl_easy_perform(curl);

		donePosting = true;
		//cout << apiPageBuffer << endl; //will print the xml file
		
		TiXmlDocument doc("friends_timeline.xml");
		bool loadOkay = doc.Parse(apiPageBuffer.c_str(), 0, TIXML_ENCODING_UTF8);
		
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
					cout << blue << screen_name->GetText() << normal << ": " << text->GetText() << endl << "tweeted on: " << created_at->GetText() << endl << endl;
				}
			}
    	}

		// Did we succeed?
		if (twitterResult == CURLE_OK){
			//cout << "SUCCESS! message sent to Twitter!" << endl; //unix style coding requires no confirmation for expected behaviours
			return;
		} else{
			cout << "OUPS! there was an error. The message was not sent to Twitter." << endl;
			return;
		}		
	}
};


class Facebook{
protected:
	string FirstName;
	string Email;
	string Password;

	string LoginPost;
	string LoginPostEmail;
	string LoginPostPassword;
	string LoginPostLogin;
	
	string finalPageBuffer;
	string frontPageBuffer;
	string formKey;
	string loginPageBuffer;
	string actionURL;

	string finalURLp1;
	string finalURL;
	
	string	postFormId;
	string	postStatus;
	string	postUpdate;
	
	string Post;
	
	size_t foundFirst;
	size_t foundLast;
	
	CURLcode facebookResult;
public:
	Facebook(){
		
	}
	
	void postMessage(string message){
		
		FirstName = "YOUR FIRST NAME";
		Email = "YOUR FACEBOOK EMAIL YOU USE TO LOGIN";
		Password = "YOUR PASSWORD";

		LoginPostEmail = "email=";
		LoginPostPassword = "&pass=";
		LoginPostLogin = "&login=Login";
		
		finalURLp1 = "http://m.facebook.com/";
		
		postFormId = "post_form_id=";
		postStatus = "&status=";
		postUpdate = "&update=Update Status";
		
		//create the login post message
		LoginPost= LoginPostEmail + Email + LoginPostPassword + Password + LoginPostLogin;

		//login to facebook mobile
		curl_easy_setopt(curl, CURLOPT_URL, "https://login.facebook.com/login.php?m&amp;next=http%3A%2F%2Fm.facebook.com%2Fhome.php");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, LoginPost.c_str() );
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &loginPageBuffer);
		curl_easy_setopt(curl, CURLOPT_HEADER, 0);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(curl, CURLOPT_COOKIE, "~/statsU_cookies.txt");
		curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "~/statsU_cookies.txt");
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "~/statsU_cookies.txt");
		curl_easy_perform(curl);
		
		//get posting form data from the front page
		curl_easy_setopt(curl, CURLOPT_POST, 0);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &frontPageBuffer);
		curl_easy_setopt(curl, CURLOPT_URL, "http://m.facebook.com/home.php");
		curl_easy_perform(curl);
		
		//find the actionable page
		foundFirst = frontPageBuffer.find("form action=\"/");
		for(int i = 0; i < 200; i++){
			if(frontPageBuffer.at(foundFirst+i+16) == '"' ){
				foundLast = foundFirst+i+16;
				foundFirst += 14;
				i = 201;
			}
		}
		
		actionURL = frontPageBuffer.substr(foundFirst, foundLast - foundFirst);
		//replace the "&amp;" with just "&" in that string
		foundFirst = actionURL.find("&");
		foundLast = actionURL.find(";") + 1;
		actionURL.replace(foundFirst, foundLast - foundFirst, "&");

		finalURL = finalURLp1 + actionURL;
		
		foundFirst = frontPageBuffer.find("name=\"post_form_id\" value=\"");
		
		for(int i = 0; i < 200; i++){
			if(frontPageBuffer.at(foundFirst+i+28) == '"'){
				foundLast = foundFirst+i+28;
				foundFirst +=  27; //26 is at the "
				i = 201;
			}
		}
		
		formKey = frontPageBuffer.substr(foundFirst, foundLast - foundFirst);
		
		Post = postFormId + formKey + postStatus + message + postUpdate;
		
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &finalPageBuffer);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Post.c_str() );
		curl_easy_setopt(curl, CURLOPT_URL, finalURL.c_str() );
		facebookResult = curl_easy_perform(curl);
		
		donePosting = true;
		
		// Did we succeed?
		if (facebookResult == CURLE_OK){
			cout << "SUCCESS! message sent to Facebook! (took it's sweet time though...)" << endl;
			return;
		}else {
			cout << "OUPS! there was an error. The message was not sent to Twitter." << endl;
			return;
		}
	}
};


int main(int argc, char* argv[]){
    try {
        po::options_description desc("**********************************************\n**Options");
        desc.add_options()
            ("help,h", "returns this help message")
            ("version,v", "returns program version")
            ("facebook,f", "post to facebook")
            ("twitter,t", "post to twitter")
			("twitter-get,g", "get twitter timeline")
        ;

        po::variables_map vm;        
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);    

        if (vm.count("help") ){
            cout << desc << endl;
            return 1;
        }

        if (vm.count("version") ){
            cout << "This twitter application is at version: 0.06" << endl;;
            return 1;
        }

		curl = curl_easy_init(); ///Create our curl handle
		if (!curl){ ///check if curl initialized correctly
			cout << "ERROR: something went wrong with curl's \"easy\" initialization, sorry!" << endl; 
			return 0;
		}
		
		if (vm.count("twitter-get") ){
			Twitter twitter;
			twitter.getTimeline();
			return 0;
		}
		
		donePosting = false;

		do {
			cout << "**your Post: ";
			getline (cin, message);
			
			if (vm.count("twitter") ){
				Twitter twitter;
				twitter.postMessage(message);
			}

			if (vm.count("facebook") ){ 
				Facebook facebook;
				facebook.postMessage(message);
			}
		} while (donePosting == false);

    }
    catch(exception& e){
        cerr << "error: " << e.what() << ". Try --help" << endl;
        return 1;
    }
    catch(...){
        cerr << "Exception of unknown type!" << endl;
    }

    return 0;
}
