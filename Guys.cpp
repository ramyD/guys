#include "Guys.h"

Guys::Guys() {
	versionNumber = 0.9; //quick way to edit version number
}

bool Guys::folderExists(std::string path) {
	FILE *pFile;
	char buffer [256];
	std::string result;
	std::string command = "cd " + path + " && pwd";

	pFile = popen(command.c_str(), "r");
	
	if (pFile == NULL) {
		std::cout << "handle error" << std::endl;
		return false;
	} else {
		while ( !feof(pFile) ) {
			fgets (buffer, 256, pFile);
		}
		pclose (pFile);
	}
	result = buffer;
	
	if ( result.length() == 6 ) {
		return false; //means the command returned junk data and is not working
	} else {
		size_t found;
		found = result.find(path.substr(2)); //substr(2) specifically for ~/ in path finding, either way it would still resolve for an absolute path (right?)
		if (found!=std::string::npos) {
			return true;
		}
	}
	
	return false;
}

bool Guys::configFileExists(std::string path) {
	FILE *pFile;
	char buffer [256];
	std::string result;
	std::string command = "cd ~/ && pwd";
	std::string file;

	pFile = popen(command.c_str(), "r");
	
	if (pFile == NULL) {
		std::cout << "handle error" << std::endl;
		return false;
	} else {
		while ( !feof(pFile) ) {
			fgets (buffer, 256, pFile);
		}
		pclose (pFile);
	}
	result = buffer;
	file = result + "/.config/" + path;
	
	std::string::iterator it;
	for ( it=file.begin() ; it < file.end(); it++ )
		if (*it == 10) file.erase(it); //erase return characters
	
	pFile = fopen (file.c_str() , "r");
	if (pFile == NULL) {
		std::cout << "config file does not exist" << std::endl;
		return false;
	} else {
		//do things to the file?
		fclose(pFile); //clos file
		return true;
	}
	
	return false;
}

void Guys::makeConfigFile(std::string fileName) {
	FILE *pFile;
	char buffer [256];
	std::string result;
	std::string command = "cd ~/ && pwd";
	std::string file;

	pFile = popen(command.c_str(), "r");
	
	if (pFile == NULL) {
		std::cout << "handle error" << std::endl;
		return;
	} else {
		while ( !feof(pFile) ) {
			fgets (buffer, 256, pFile);
		}
		pclose (pFile);
	}
	result = buffer;
	file = result + "/.config/" + fileName;
	
	configPath = result + "/.config/guys/"; //accessible to all functions
	
	std::string::iterator it;
	for ( it=file.begin() ; it < file.end(); it++ )
		if (*it == 10) file.erase(it); //erase return characters
	
	pFile = fopen (file.c_str() , "w");
	if (pFile == NULL) {
		std::cout << "file creation error" << std::endl;
		return;
	} else {
		//do things to the file?
		fclose(pFile); //close file
		return;
	}
	
	return;
}

void Guys::makeConfigDir(std::string folderName) {
	FILE *pFile;
	char buffer [256];
	std::string result;
	std::string command = "cd ~/ && pwd";
	std::string file;

	pFile = popen(command.c_str(), "r");
	
	if (pFile == NULL) {
		std::cout << "handle error" << std::endl;
		return;
	} else {
		while ( !feof(pFile) ) {
			fgets (buffer, 256, pFile);
		}
		pclose (pFile);
	}
	result = buffer;
	file = result + "/.config/" + folderName;
	command = "mkdir " + file;
	
	std::string::iterator it;
	for ( it=command.begin() ; it < command.end(); it++ )
		if (*it == 10) command.erase(it); //erase return characters
	
	pFile = popen (command.c_str() , "r");
	if (pFile == NULL) {
		std::cout << "process creation error" << std::endl;
		return;
	} else {
		pclose(pFile); //close file
		return;
	}
	
	return;
}


void Guys::configurationFileCheck() {
	if( configFileExists("guys/twitter") ) {
		//std::cout << "file exists" << std::endl;
		
		FILE *pFile;
		char buffer [100];
		std::string result;
		std::string command = "cd ~/ && pwd";
		std::string file;

		pFile = popen(command.c_str(), "r");
		
		if (pFile == NULL) {
			std::cout << "handle error" << std::endl;
			return;
		} else {
			while ( !feof(pFile) ) {
				fgets (buffer , 100 , pFile);
			}
			pclose (pFile);
		}
		result = buffer;
		configPath = result + "/.config/guys/"; //accessible to all functions
		
		std::string::iterator it;
		for ( it=configPath.begin() ; it < configPath.end(); it++ )
			if (*it == 10) configPath.erase(it); //erase return characters
		
	} else {
		if( folderExists("~/.config/guys") ) {
			std::cout << "creating config files in ~/.config/guys/" << std::endl; //but not the file
			makeConfigFile("guys/twitter"); //create the file
			configurationFileCheck();
		} else {
			makeConfigDir("guys");
			configurationFileCheck();
		}
	}
	
	return;
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
		twitter->post(quickPost.str(), configPath);
	}

/*
	//facebook now has it's own Oauth implementation which we now need to investigate -_-
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
*/
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
		twitter->getTimeline(configPath);
	}

	return 0;
}

int Guys::displayHelp() {
	std::cout << "Guys ********************************************" << std::endl;
	std::cout << "**Options:" << std::endl;
	std::cout << "  -h [ --help ]         returns this help message" << std::endl;
	std::cout << "  -v [ --version ]      returns program version" << std::endl;
	std::cout << "  -f [ --facebook ]     [disabled] post to facebook" << std::endl;
	std::cout << "  -t [ --twitter ]      post to twitter" << std::endl;
	std::cout << "  -a [ --all ]          [disabled] post to all services" << std::endl;
	std::cout << "  -g [ --twitter-get ]  get twitter timeline" << std::endl;
	std::cout << "*************************************************" << std::endl;
	std::cout << "**Notes:" << std::endl;
	std::cout << "oauth keys are stored under ~/.config/guys/, if you wish" << std::endl;
	std::cout << "to reset your keys, simply delete the files inside or the" << std::endl;
	std::cout << "the entire guys directory." << std::endl;
	std::cout << "*************************************************" << std::endl;
	std::cout << "**Version " << versionNumber << std::endl;

	return 0;
}

int Guys::displayVersion() {
	std::cout << "Guys updates your status, version: "<< versionNumber << std::endl;

	return 0;
}
