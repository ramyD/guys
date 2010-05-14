/*
 * 
 * g++ -Wall -std=c++98 -pedantic -Os tinyxml.cpp tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp Bitly.cpp shortener_0.01.cpp -lcurl -o shortener
 * 
 * */

#include "Bitly.h"

int main(int argc, char* argv[]){
	Bitly *bitly = new Bitly;
	bitly->interpretArguments(argc, argv); //interprets and acts on all arguments
	return 0;
}
