#include "Twitter.h"

Twitter::Twitter(){ //we put the constructor at the top so the we can quickly see and edit user credentials
	//login credentisl
    UserName = "";
    Password = "";
    Account = UserName + ':' + Password;

    if (UserName.length() == 0){
        std::cout << "you can not use Twitter, no login data specified" << std::endl;
        return;
    }

    curl = curl_easy_init();
}

// This is the writer call back function used by curl
static int twitterWriter(char *data, size_t size, size_t nmemb, std::string *buffer){
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

void Twitter::post(std::string message){
	if (message.size() <= 0) { //if there is no quick post
		std::cout << "**your Post: ";
		getline (std::cin, message);
	} else { //if there is a quick post
		std::cout << "**your Post: " << message << std::endl;
	}
	
	std::string		prependPost;
	std::string		statusUpdate;

    prependPost = "status=";
    statusUpdate = prependPost + message;
    
    URL = "http://twitter.com/statuses/update.xml";
    
    if (message.length() > 140){
        if ( (message.length() - 140) < 2){
            std::cout << "OUPS! message is 1 character above the 140 character limit" << std::endl;
        } else {
            std::cout << "OUPS! message is "<< message.length() - 140 << " characters above the limit" << std::endl;
        }
    } else {
        ///Now set up all of the curl options
        curl_easy_setopt(curl, CURLOPT_USERPWD, Account.c_str() );
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, twitterWriter);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &apiPageBuffer);
        curl_easy_setopt(curl, CURLOPT_POST, 1 );
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, statusUpdate.c_str() );
        
        curl_easy_setopt(curl, CURLOPT_URL, URL.c_str() );
        
        //cout << "sending..." << endl; //unix style coding requires no confirmation for expected behaviours
        twitterResult = curl_easy_perform(curl);

        // Did we succeed?
        if (twitterResult == CURLE_OK){
            //std::cout << "SUCCESS! message sent to Twitter!" << std::endl; //unix style coding requires no confirmation for expected behaviours
            return;
        } else{
            std::cout << "OUPS! there was an error. The message was not sent to Twitter." << std::endl;
            return;
        }
    }
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


void Twitter::getTimeline(void){
    URL = "http://twitter.com/statuses/friends_timeline.xml";

    // Now set up all of the curl options
    curl_easy_setopt(curl, CURLOPT_USERPWD, Account.c_str() );
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, twitterWriter);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &apiPageBuffer);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1 );
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    
    curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
    
    //cout << "sending..." << endl; //unix style coding requires no confirmation for expected behaviours
    twitterResult = curl_easy_perform(curl);

    //std::cout << apiPageBuffer << std::endl; //will print the xml file
    
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
                std::cout << blue << screen_name->GetText() << normal << ": " << text->GetText() << std::endl << "tweeted on: " << created_at->GetText() << std::endl << std::endl;
            }
        }
    }

    // Did we succeed?
    if (twitterResult == CURLE_OK){
        //std::cout << "SUCCESS! message sent to Twitter!" << std::endl; //unix style coding requires no confirmation for expected behaviours
        return;
    } else{
        std::cout << "OUPS! there was an error. The message was not sent to Twitter." << std::endl;
        return;
    }
}
