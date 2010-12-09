/*
 * Logger.cpp
 *
 *  Created on: Dec 9, 2010
 *      Author: bmonkey
 */

#include <iostream>
#include <sstream>
#include "Logger.h"
#include "BashColor.h"

Logger::Logger() {
	// TODO Auto-generated constructor stub

}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

void Logger::log(string name, string message ,LogType type){
	string messageColor, typeString;

	switch(type){
		case ERROR:
			typeString = "ERROR";
			cerr 	<< bashColor(typeString, composeColor(bold, red))
					<< bashColor(name, composeColor(underline, white))
					<< " - " << message << "\n";
			exit(0);
			break;
		case WARNING:
			typeString = "WARNING";
			messageColor = composeColor(bold, yellow);
		case MESSAGE:
			typeString = "MESSAGE";
			messageColor = composeColor(regular, green);
		case DEBUG:
			typeString = "DEBUG";
			messageColor = composeColor(regular, purple);
		default:
			messageColor = composeColor(regular, purple);
			break;
	}

	cout 	<< bashColor("["+typeString+"]", messageColor)
			<< bashColor(name, composeColor(underline, white))
			<< " - " << message << "\n";
}

string Logger::bashColor(string message, string color){

	return "\x1b[" +  color + "m" + message + "\e[m";

}

string Logger::composeColor(int background, int foreground){
	stringstream color;
	if (background > 29) background+=10;
	color << background << ";" << foreground;
}
