/*
 * Logger.cpp
 *
 *  Created on: Dec 9, 2010
 *      Author: bmonkey
 */

#include <iostream>

#include "Logger.h"
#include "BashColor.h"

Logger::Logger() {
	// TODO Auto-generated constructor stub

}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

void Logger::log(LogType type){
	log(type, "");
}

void Logger::log(LogType type, string name){
	string messageColor, typeString;

	switch(type){
		case LOG_ERROR:
			typeString = "ERROR";
			cerr 	<< bashColor(typeString, composeColor(bold, red))
					<< bashColor(name, composeColor(underline, white))
					<< " - " << message.str() << "\n";
			exit(0);
			break;
		case LOG_WARNING:
			typeString = "WARNING";
			messageColor = composeColor(bold, yellow);
		case LOG_MESSAGE:
			typeString = "MESSAGE";
			messageColor = composeColor(regular, green);
		case LOG_DEBUG:
			typeString = "DEBUG";
			messageColor = composeColor(regular, purple);
		default:
			messageColor = composeColor(regular, purple);
			break;
	}

	cout 	<< bashColor("["+typeString+"]", messageColor)
			<< bashColor(name, composeColor(underline, white))
			<< " - " << message.str() << "\n";
	message.str("");
}

string Logger::bashColor(string message, string color){

	return "\x1b[" +  color + "m" + message + "\e[m";

}

string Logger::composeColor(int background, int foreground){
	stringstream color;
	if (background > 29) background+=10;
	color << background << ";" << foreground;
	return color.str();
}
