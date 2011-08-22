/*
 * Timer.h
 *
 *  Created on: Apr 10, 2011
 *      Author: bmonkey
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <time.h>

#include "common/Singleton.h"
#include "Window/Input.h"

class Timer : public Singleton<Timer>{
private:
#ifndef LIBLUB_WINDOWS
  timespec startTime, lastTime, frameTime;
  timespec elapsed(timespec &start, timespec &end);
#else
  //TODO: Qt Timer for Win
#endif

public:
  Timer();
  virtual ~Timer();
  void updateFPS();
  void printFPS();
  float getFPS();
  float getSPF();
  float getTime();
};

#endif /* TIMER_H_ */
