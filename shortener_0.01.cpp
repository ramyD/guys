/*
 * 
 * g++ -Wall -std=c++98 -pedantic -Os tinyxml.cpp tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp Bitly.cpp shortener_0.01.cpp -lcurl -o shortener
 * 
 * */

#include "Bitly.h"

void wait ( int seconds )
{
  clock_t endwait;
  endwait = clock () + seconds * CLOCKS_PER_SEC ;
  while (clock() < endwait) {}
}


int main(int argc, char* argv[]){
	Bitly *bitly = new Bitly;
    //std::cout << bitly->shorten(argv[1]) << std::endl; //interprets and acts on all arguments
    
    std::cout << bitly->checkForUrl("hello alexis www.alexismorin.com iuahe") <<  std::endl;
    std::cout << bitly->checkForUrl("hello alexis www.alexismorin.com http://www.google.com iuahe") <<  std::endl;
    std::cout << bitly->checkForUrl("oiaei ah siuhd ah h dskja hskdh http://www.reddit.com iuaheiuahiueh ish sh") <<  std::endl;
    std::cout << bitly->checkForUrl("oiaei ah siuhd ah h dskja hskdh http://www.youtube.com iuaheiuahiueh ish sh") <<  std::endl;
    std::cout << bitly->checkForUrl("oiaei ah siuhd ah h dskja hskdh http://www.gmail.com") <<  std::endl;
    
	return 1;
}
