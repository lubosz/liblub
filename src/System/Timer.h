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
public:
  unsigned fps_current;  // the current FPS.
  unsigned fps_lasttime;  // the last recorded time.
  unsigned input_lasttime;  // the last recorded time.
  unsigned fps_frames;  // frames passed since the last recorded fps.
  unsigned ticks;
  unsigned secoundsPassed;
  unsigned nanosecoundsPassed;
#ifndef LIBLUB_WINDOWS
  timespec start;
#endif
  Timer();
  virtual ~Timer();
  void frame(Input * input);
  void frame();
private:
  void checkFPS();
  void tick();
};

#endif /* TIMER_H_ */
