/*
 * Timer.cpp
 *
 *  Created on: Apr 10, 2011
 *      Author: bmonkey
 */

#include "System/Timer.h"
//#include "Window/Window.h"
#include "System/Logger.h"

Timer::Timer() {
    framesRendered = 0;
    avarageFrameTimeMs = 0;
#ifndef LIBLUB_WINDOWS
  clock_gettime(CLOCK_MONOTONIC, &lastTime);
#endif
}

Timer::~Timer() {
  // TODO Auto-generated destructor stub
}

#ifndef LIBLUB_WINDOWS

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

void Timer::startFrame() {
  clock_gettime(CLOCK_MONOTONIC, &lastTime);
}

void Timer::frameDone() {
  timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  frameTime = elapsed(lastTime, now);
  lastTime = now;
}

float Timer::getFPS() {
  return BILLION / float(frameTime.tv_nsec);
}

float Timer::getMilliseconds() {
  return float(frameTime.tv_nsec) / MILLION;
}

float Timer::getSeconds() {
  return float(frameTime.tv_sec) + getMilliseconds()/1000.0;
}

float Timer::getTime() {
  timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return (float(now.tv_sec) + float(now.tv_nsec)/BILLION);
}

#else


void Timer::startFrame() {
}
void Timer::frameDone() {
}
float Timer::getSeconds() {
    return 0;
}
float Timer::getMilliseconds() {
    return 0;
}

float Timer::getFPS() {
  return 0;
}

float Timer::getTime() {
    return 0;
}

#endif

void Timer::printFPS() {
  LogInfo << "FPS" << getFPS() << "," << getMilliseconds() << "ms per Frame";
}

void Timer::countAverage() {
    framesRendered++;
    float frameTimeMs = getMilliseconds();
    if (avarageFrameTimeMs == 0) {
        avarageFrameTimeMs = frameTimeMs;
    } else {
        avarageFrameTimeMs = ((avarageFrameTimeMs * (framesRendered - 1)) + frameTimeMs) / framesRendered;
//        avarageFrameTimeMs = (avarageFrameTimeMs + frameTimeMs) / 2.0;
    }
}
