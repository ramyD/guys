#
# Makefile for Guys 0.07
# author: ramy.daghstani@gmail.com
# 

#not the best makefile, still missing some things!
#using this as a guide: http://www.wlug.org.nz/MakefileHowto

CXXFLAGS=-Wall -std=c++98 -pedantic -Os

guys: tinyxml.cpp tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp Guys.cpp Facebook.cpp Twitter.cpp guys0_07.cpp
	g++ $(LDFLAGS) tinyxml.cpp tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp Guys.cpp Facebook.cpp Twitter.cpp guys0_07.cpp -lcurl -o guys

#can't get it to work. Oh well!
#guys: tinyxml.o guys.o
#	g++ $(LDFLAGS) tinyxml.o guys.o -lcurl -o guys

#guys.o: Guys.cpp Guys.h Facebook.cpp Facebook.h Twitter.cpp Twitter.h guys007.cpp Makefile
#tinyxml.o: tinyxml.cpp tinyxml.h tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp tinystr.h Makefile

