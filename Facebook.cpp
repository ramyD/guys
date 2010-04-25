#include "Facebook.h"

Facebook::Facebook(){
	FirstName = "";
	Email = "";
	Password = "";
	
	if (Email.length() == 0){
        std::cout << "you can not use Facebook, no login data specified" << std::endl;
        return;
    }
	
	curl = curl_easy_init();
}

// This is the writer call back function used by curl
static int facebookWriter(char *data, size_t size, size_t nmemb, std::string *buffer){
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

void Facebook::post(std::string message){
	if (message.size() <= 0) { //if there is no quick post
		std::cout << "**your Post: ";
		getline (std::cin, message); //not sure if this works
	} else { //if there is a quick post
		std::cout << "**your Post: " << message << std::endl;
	}
	
	
	
	
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
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, facebookWriter);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &loginPageBuffer);
	curl_easy_setopt(curl, CURLOPT_HEADER, 0);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_COOKIE, "~/statsU_cookies.txt");
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "~/statsU_cookies.txt");
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "~/statsU_cookies.txt");
	curl_easy_perform(curl);
	
	//get posting form data from the front page
	curl_easy_setopt(curl, CURLOPT_POST, 0);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, facebookWriter);
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
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, facebookWriter);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &finalPageBuffer);

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Post.c_str() );
	curl_easy_setopt(curl, CURLOPT_URL, finalURL.c_str() );
	facebookResult = curl_easy_perform(curl);
	
	
	// Did we succeed?
	if (facebookResult == CURLE_OK){
		std::cout << "SUCCESS! message sent to Facebook! (took it's sweet time though...)" << std::endl;
		return;
	}else {
		std::cout << "OUPS! there was an error. The message was not sent to Twitter." << std::endl;
		return;
	}
}
