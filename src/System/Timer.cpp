/*
 * Timer.cpp
 *
 *  Created on: Apr 10, 2011
 *      Author: bmonkey
 */

#include "System/Timer.h"
#include "System/GUI.h"
#include "Window/MediaLayer.h"

Timer::Timer() {
  fps_lasttime = 0;  // the last recorded time.
  fps_frames = 0;  // frames passed since the last recorded fps.
  input_lasttime = 0;
  secoundsPassed = 0;
  nanosecoundsPassed = 0;
#ifndef WITH_SDL
  clock_gettime(CLOCK_MONOTONIC, &start);
#endif
}

Timer::~Timer() {
  // TODO Auto-generated destructor stub
}


void Timer::frame() {

#ifndef WITH_SDL
  timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);

  secoundsPassed = now.tv_sec - start.tv_sec;
  nanosecoundsPassed = now.tv_nsec;

  ticks = now.tv_sec * 1000 + now.tv_nsec / 1000000;
#else
  ticks = SDL_GetTicks();
#endif
  fps_frames++;

  //check input every 1/100 secound
    if (input_lasttime < ticks - 10) {
      input_lasttime = ticks;
      MediaLayer::Instance().input->eventLoop();
      GUI::Instance().update();
    }

   //reset fps counter every secound
  if (fps_lasttime < ticks - 1000) {
    fps_lasttime = ticks;
    fps_current = fps_frames;
    fps_frames = 0;
  }
}
