#include <QtCore>
#include <string>
#include <iostream>
#include "System/Logger.h"
#include "Common/BashColor.h"
#include "Renderer/OpenGL.h"

using std::cerr;
using std::cout;

Logger::LogLevel Logger::logLevel = Logger::Debug;

struct LogSetting {
  string codefile;
  Logger::LogLevel logLevel;
};

const LogSetting blackList[] = {
  { "Timer", Logger::Info },
  { "TemplateEngine", Logger::Info },
  { "Shader", Logger::Info },
  { "ShaderProgram", Logger::Info },
  { "Config", Logger::Info },
  { "Mesh", Logger::Info },
  { "MeshBuffer", Logger::Info },
  { "XCBApplication", Logger::Info },
  { "SceneGraph", Logger::Info },
  { "SceneLoader", Logger::Info },
  { "Textures", Logger::Info },
  { "Texture", Logger::Info },
  { "FrameBuffer", Logger::Info },
  { "AssimpSceneLoader", Logger::Info },
  { "MeshLoader", Logger::Info },
  { "Geometry", Logger::Info },
  { "DeferredRenderer", Logger::Info },
  { "Scene", Logger::Info },
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
  if (fatal) {
      // generate segfault for backtrace
      int *ptr = nullptr;
      *ptr = 1;
      exit(0);
  }
}

void Logger::writeInfo(const char* longFile, int line, Logger::LogLevel level) {
  string file = std::strrchr(longFile, '/')+1;
  file = file.substr(0,file.size()-4);

  fatal = false;
  LogLevel curLevel = getLogLevel(file);
  if(level < curLevel) {
    print = false;
    return;
  }

  print = true;
  switch(level) {
    case Info:
      log(1,32,"I",file, line);
      break;
    case Warning:
      log(1,33,"W",file, line);
      break;
    case Error:
      log(1,31,"E",file, line);
      break;
    case Debug:
      log(1,36,"D",file, line);
      break;
    case Fatal:
      log(4,31,"FATAL",file, line);
      fatal = true;
      break;
    default:
      log(1,32,"I",file, line);
  };
}

void Logger::log(int mode, int color, const string & level,
    const string & file, int line) {
    const unsigned alignedTextSize = 11;
#if LIBLUB_WINDOWS
  cout<<"["<< level << "]  "<<file<<":"<<line<<"  ";
#else
  cout<<"\e["<< mode <<";"<< color <<"m[" << level << "]\e[m "
      <<"\e[1;34m"<<file<<" \e[0;33m"<<line<<"\e[m"<< getSpaces(alignedTextSize - file.size(), line);
#endif
}

string Logger::getSpaces(int count, int line) {
    string spaces = " ";

    if (line < 100)
        spaces += " ";

    for (int i = 0; i < count; i++) {
        spaces += " ";
    }
    return spaces;
}

Logger::LogLevel Logger::getLogLevel(const string& file) {
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
