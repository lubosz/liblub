/*
 * Timer.cpp
 *
 *  Created on: Apr 10, 2011
 *      Author: bmonkey
 */

#include "System/Timer.h"
#include "System/GUI.h"
#include "Window/Window.h"

Timer::Timer() {
  clock_gettime(CLOCK_MONOTONIC, &startTime);
}

Timer::~Timer() {
  // TODO Auto-generated destructor stub
}

#ifndef LIBLUB_WINDOWS
const int BILLION = 1000000000; // 10^9

/* Returns a timespec representing the elapsed time between
 * start and end.
 * Assumption: end is bigger (later) than start. */
timespec Timer::elapsed(timespec &start, timespec &end) {
    timespec result;
    if(end.tv_nsec < start.tv_nsec) { // borrow from tv_sec
        result.tv_sec = (end.tv_sec - 1) - start.tv_sec;
        result.tv_nsec = (end.tv_nsec + BILLION) - start.tv_nsec;
    } else {
        result.tv_sec = end.tv_sec - start.tv_sec;
        result.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return result;
}

void Timer::updateFPS() {
  timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  frameTime = elapsed(lastTime, now);
  lastTime = now;
}

float Timer::getFPS() {
  float fps = BILLION / float(frameTime.tv_nsec);
}

float Timer::getSPF() {
  float ms = float(frameTime.tv_nsec) / 1000000.0;
}
#endif

void Timer::printFPS() {
  LogInfo << "FPS" << getFPS() << "," << getSPF() << "ms per Frame";
}

float Timer::getTime() {
  timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return (float(now.tv_sec) + float(now.tv_nsec)/float(BILLION));
}
