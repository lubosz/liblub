/*
 * Timer.h
 *
 *  Created on: Apr 10, 2011
 *      Author: bmonkey
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <time.h>

#include "Window/Input.h"

const float BILLION = 1000000000; // 10^9
const float MILLION = 1000000;

class Timer {
public:
#ifndef LIBLUB_WINDOWS
  timespec startTime, lastTime, frameTime;
  static timespec elapsed(timespec &start, timespec &end);
#else
  //TODO: Qt Timer for Win
#endif

  Timer();
  virtual ~Timer();
  void updateFPS();
  void printFPS();
  float getFPS();
  float getSPF();
  static float getTime();
};

#endif /* TIMER_H_ */
