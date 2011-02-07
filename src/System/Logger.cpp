/*
 * Copyright © 2010 Lubosz Sarnecki
 * Logger.cpp
 *
 *  Created on: Dec 9, 2010
 */

#include <QtCore>
#include <string>
#include <iostream>
#include "System/Logger.h"
#include "common/BashColor.h"
#include "common/OpenGL.h"

using std::cerr;
using std::cout;

Logger::Logger() {
  blacklist = {
      "Light Position",
      "Mesh",
      "FBO"
  };
}

Logger::~Logger() {
  // TODO(bmonkey): Auto-generated destructor stub
}

void Logger::log(string type) {
  log(type, "");
}

void Logger::log(string type, string name, string say) {
  message << say;
  log(type, name);
}

void Logger::log(string type, string name) {
  foreach(string blackname, blacklist) {
    if (blackname == name) {
      clear();
      return;
    }
  }

  string messageColor;

  if (type == "ERROR") {
      cerr   << bashColor("["+type+"]", composeColor(bold, red))
          << " - "
          << bashColor(name, composeColor(underline, white)) << " - "
          << message.str() << "\n";
      exit(0);
  } else if (type == "WARNING") {
    messageColor = composeColor(bold, yellow);
  } else if (type == "MESSAGE") {
    messageColor = composeColor(regular, green);
  } else if (type == "DEBUG") {
    messageColor = composeColor(regular, purple);
  } else {
    messageColor = composeColor(regular, cyan);
  }


  cout   << bashColor("["+type+"]", messageColor)
      << " - ";
  if (name != "")
    cout  << bashColor(name, composeColor(underline, white)) << " - ";
  cout  << message.str() << "\n";
  clear();
}

void Logger::clear() {
  message.str("");
}

string Logger::bashColor(string message, string color) {
  return "\x1b[" +  color + "m" + message + "\e[m";
}

string Logger::composeColor(int background, int foreground) {
  stringstream color;
  if (background > 29) background+=10;
  color << background << ";" << foreground;
  return color.str();
}



void Logger::checkGlError(const char* file, int line) {
  GLenum err(glGetError());

  while (err != GL_NO_ERROR) {
    string error;

    switch (err) {
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
            default:
                error = "Unknown error";
                break;
    }

    message <<
        "GL_" << error << " - " << file << ':' << line;
      log("ERROR", "glError");
    exit(0);
  }
}
