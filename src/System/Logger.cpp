#include <QtCore>
#include <string>
#include <iostream>
#include "System/Logger.h"
#include "common/BashColor.h"
#include "common/OpenGL.h"

using std::cerr;
using std::cout;

#define BASH_COLOR 1

Logger::LogLevel Logger::logLevel = Logger::Debug;

struct LogSetting {
  string codefile;
  Logger::LogLevel logLevel;
};

const LogSetting blackList[] = {
  { "Mesh.cpp", Logger::Warning },
};

Logger::Logger(const std::string& file, int line, Logger::LogLevel level) {
  writeInfo(file.c_str(), line, level);
}

Logger::Logger(const char* file, int line, Logger::LogLevel level) {
  writeInfo(file, line, level);
}

Logger::~Logger() {
  if (print)
    cout<<std::endl;
  if (fatal)
    exit(0);
}

void Logger::writeInfo(const char* longFile, int line, Logger::LogLevel level) {
  const char* file = std::strrchr(longFile, '/')+1;
  if(file == 0)
    file = std::strrchr(longFile, '\\')+1;

  fatal = false;
  LogLevel curLevel = getLogLevel(file);
  if(level < curLevel) {
    print = false;
    return;
  }

  print = true;
  switch(level) {
    case Info:
      log(1,32,"INFO",file, line);
      break;
    case Warning:
      log(1,33,"WARNING",file, line);
      break;
    case Error:
      log(1,31,"ERROR",file, line);
      break;
    case Debug:
      log(1,36,"DEBUG",file, line);
      break;
    case Fatal:
      log(4,31,"FATAL",file, line);
      fatal = true;
      break;
    default:
      log(1,32,"INFO",file, line);
  };
}

void Logger::log(int mode, int color, const string & level,
    const string & file, int line) {
#if BASH_COLOR
  cout<<"[\e["<< mode <<";"<< color <<"m" << level << "\e[m]  "
      <<"\e[0;35m"<<file<<"\e[m:\e[0;33m"<<line<<"\e[m"<<"  ";
#else
  cout<<"["<< level << "]  "<<file<<":"<<line<<"  ";
#endif
}

Logger::LogLevel Logger::getLogLevel(const std::string& file) {
  for (unsigned i=0; i < sizeof(blackList)/sizeof(*blackList); i++) {
    if (blackList[i].codefile == file)
      return blackList[i].logLevel;
  }
  return logLevel;
}

Logger & Logger::operator<<(const nullstr & s) {
  if(print) {
    if(s.str) {
      *this << "\"" << s.str << "\"";
    } else {
      *this << "NULL";
    }
  }
  return *this;
}
