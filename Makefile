#
# Makefile for Guys 0.08
# author: ramy.daghstani@gmail.com
# 

#not the best makefile, still missing some things!
#using this as a guide: http://www.wlug.org.nz/MakefileHowto

CXXFLAGS=-Wall -std=c++98 -pedantic -Os

guys: tinyxml.cpp tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp format.cpp Oauth.cpp Guys.cpp Facebook.cpp Twitter.cpp Bitly.cpp guys1_0.cpp
	g++ $(LDFLAGS) $(CXXFLAGS) tinyxml.cpp tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp format.cpp Oauth.cpp Guys.cpp Facebook.cpp Twitter.cpp Bitly.cpp guys1_0.cpp -lcurl -lcrypto++ -o guys

clean: 
	rm ./guys

debug: tinyxml.cpp tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp format.cpp Oauth.cpp Guys.cpp Facebook.cpp Twitter.cpp Bitly.cpp guys1_0.cpp
	g++ $(LDFLAGS) $(CXXFLAGS) -g tinyxml.cpp -g tinyxmlparser.cpp -g tinyxmlerror.cpp -g tinystr.cpp -g format.cpp -g Oauth.cpp -g Guys.cpp -g Facebook.cpp -g Twitter.cpp -g Bitly.cpp -g guys1_0.cpp -g -lcurl -g -lcrypto++ -o guys


