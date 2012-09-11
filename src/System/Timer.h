/*
 * Timer.h
 *
 *  Created on: Apr 10, 2011
 *      Author: bmonkey
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <time.h>

const float BILLION = 1000000000; // 10^9
const float MILLION = 1000000;

class Timer {
public:
#ifndef LIBLUB_WINDOWS
  timespec lastTime, frameTime;
  static timespec elapsed(timespec &start, timespec &end);
#else
  //TODO: Qt Timer for Win
#endif

    unsigned framesRendered;
    float avarageFrameTimeMs;

  Timer();
  virtual ~Timer();
  void startFrame();
  void frameDone();
  void printFPS();
  float getSeconds();
  float getFPS();
  float getMilliseconds();
  static float getTime();
  void countAverage();
};

#endif /* TIMER_H_ */
