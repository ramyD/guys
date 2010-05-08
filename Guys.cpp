#include "Guys.h"

Guys::Guys() {
	versionNumber = 0.07; //quick way to edit version number
}

int Guys::interpretArguments(int argc, char* argv[]) {
	if (argc < 2) { //if no arguments
		displayHelp();
		return 0;
	}
	
	std::ostringstream unformattedArguments;
	std::vector <std::string> arguments;
	for (int i = 0; i < argc; i++){  //assign each argument to a string array alled "arguments"
		unformattedArguments << argv[i];
		arguments.push_back(unformattedArguments.str());
		unformattedArguments.str("");
	}

	if (arguments[1] == "-h" || arguments[1] == "--help") {
		displayHelp();
		return 0;
	}

	if (arguments[1] == "-v" || arguments[1] == "--version") {
		displayHelp();
		return 0;
	}
    
    //get the users input
    std::ostringstream quickPost;
    
    /*
     *here, parse the user's input in order to do some regexps and use URL shortener
     */

	if (arguments[1] == "-t" || arguments[1] == "--twitter") {
        Twitter *twitter = new Twitter;
		if (argc > 2) { //if the user inputed his post right after putting -t either by accident or on purpouse
			for (int argumentNumber=2; argumentNumber<argc-1; argumentNumber++) { //we start at 2 because we want everything posted after the -t and not before
				//we stop at argc-1 because the last input must not have a space after it lest we use an extra character for nothing.
				quickPost << arguments[argumentNumber];
				quickPost << " "; //add a space after each input or else thewordswillbestucktogether
			}
			quickPost << arguments[argc-1]; //we do this make sure the last input into the stream doesn't have a space at the end that takes up 1 character for nothing!
		}
		twitter->post(quickPost.str());
	}

	if (arguments[1] == "-f" || arguments[1] == "--facebook") {
		//std::ostringstream quickPost;
		Facebook *facebook = new Facebook;
		if (argc > 2) { //if the user inputed his post right after putting -f either by accident or on purpouse
			for (int argumentNumber=2; argumentNumber<argc-1; argumentNumber++) { //we start at 2 because we want everything posted after the -t and not before
				//we stop at argc-1 because the last input must not have a space after it lest we use an extra character for nothing.
				quickPost << arguments[argumentNumber];
				quickPost << " "; //add a space after each input or else thewordswillbestucktogether
			}
			quickPost << arguments[argc-1]; //we do this make sure the last input into the stream doesn't have a space at the end that takes up 1 character for nothing!
		}
		facebook->post(quickPost.str());
	}

	if (arguments[1] == "-a" || arguments[1] == "--all") {
		/* we will do this one laterrr
		std::ostringstream quickPost;
		Facebook facebook = new facebook;
		Twitter twitter = new Twitter;
		if (argc > 2) {
			for (int argumentNumber=2; argumentNumber<argc-1; argumentNumber++) {
				quickPost << arguments[argumentNumber];
				quickPost << " ";
			}
			quickPost << arguments[argc-1];
		}
		facebook->post(quickPost.str());
		twitter->post(quickPost.str());
		*/
	}

	if (arguments[1] == "-g" || arguments[1] == "--twitter-get") {
		Twitter *twitter = new Twitter;
		twitter->getTimeline();
	}

	return 0;
}

int Guys::displayHelp() {
	std::cout << "Guys ********************************************" << std::endl;
	std::cout << "**Options:" << std::endl;
	std::cout << "  -h [ --help ]         returns this help message" << std::endl;
	std::cout << "  -v [ --version ]      returns program version" << std::endl;
	std::cout << "  -f [ --facebook ]     post to facebook" << std::endl;
	std::cout << "  -t [ --twitter ]      post to twitter" << std::endl;
	std::cout << "  -a [ --all ]          post to all services" << std::endl;
	std::cout << "  -g [ --twitter-get ]  get twitter timeline" << std::endl;
	std::cout << "*************************************************" << std::endl;
	std::cout << "**Version " << versionNumber << std::endl;

	return 0;
}

int Guys::displayVersion() {
	std::cout << "Guys updates your status, version: "<< versionNumber << std::endl;

	return 0;
}
