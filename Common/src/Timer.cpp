/*---[Logger.h]------------------------------------------------m(._.)m-------*\
|
|  KnoRBA Common
|
|  Copyright (c) 2013, RIKEN (The Institute of Physical and Chemial Research)
|  All rights reserved.
|
|  Author: Hamed KHANDAN (hkhandan@ieee.org)
|
|  This file is distributed under the KnoRBA Free Public License. See
|  LICENSE.TXT for details.
|
*//////////////////////////////////////////////////////////////////////////////

/** 
 *
 * @file
 * This file implements `org::knorba::common::Timer` class.
 *
 */

#include "Timer.h"

namespace org {
namespace knorba {
namespace common {

Timer::Timer()
{
  Timer("");
}

Timer::Timer(string name) {
  _name = name;
  _clockBase = 0;
}

void Timer::start() {
  _clockBase = clock();
  gettimeofday(&_timeBase, NULL);
}

bool Timer::isStarted() {
  return _clockBase != 0;
}

double Timer::get() {
  if(!isStarted())
    return NAN;

  struct timeval now;
  gettimeofday(&now, NULL);
  double diff = now.tv_sec - _timeBase.tv_sec 
              + (now.tv_usec - _timeBase.tv_usec) / 1000000.0;
  return diff;
}

double Timer::getCpuTime() {
  return (clock() - _clockBase) / (double)CLOCKS_PER_SEC; 
}

void Timer::printToStream(ostream& os) {
  os << "Timer[";
  if(_name.size() != 0)
    os << "name: \"" << _name << "\", ";
  if(!isStarted())
    os << "NOT_STARTED]";
  else
    os << "elapsed: " << get() << ", cpuTime: " << getCpuTime() << "]";
}

} // namespace common
} // namespace knorba
} // namespace org


