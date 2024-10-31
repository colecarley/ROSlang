
all: roslang

roslang: lexer.l parser.y main.cpp
	bison -d -o parser.cpp parser.y -v
	flex -o lexer.cpp lexer.l     
	g++ -std=c++11 -o roslang main.cpp parser.cpp lexer.cpp

clean: 
	rm -f roslang parser.cpp parser.hpp lexer.cpp parser.output
