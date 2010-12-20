/*
 * Logger.cpp
 *
 *  Created on: Dec 9, 2010
 *      Author: bmonkey
 */

#include <iostream>
#include <QtCore>
#include "Logger.h"
#include "BashColor.h"

Logger::Logger() {
	blacklist = {
			"Light Position",
			"Mesh",
			"FBO"
	};

}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

void Logger::log(string type){
	log(type, "");
}

void Logger::log(string type, string name, string say){
	message << say;
	log(type, name);
}

void Logger::log(string type, string name){
	foreach(string blackname, blacklist){
		if (blackname == name){
			clear();
			return;
		}
	}

	string messageColor;

	if (type == "ERROR"){
			cerr 	<< bashColor("["+type+"]", composeColor(bold, red))
					<< " - "
					<< bashColor(name, composeColor(underline, white)) << " - "
					<< message.str() << "\n";
			exit(0);
	}else if(type == "WARNING"){
		messageColor = composeColor(bold, yellow);
	}else if(type == "MESSAGE"){
		messageColor = composeColor(regular, green);
	}else if(type == "DEBUG"){
		messageColor = composeColor(regular, purple);
	}else{
		messageColor = composeColor(regular, cyan);
	}


	cout 	<< bashColor("["+type+"]", messageColor)
			<< " - ";
	if (name != "")
		cout	<< bashColor(name, composeColor(underline, white)) << " - ";
	cout	<< message.str() << "\n";
	clear();
}

void Logger::clear(){
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
