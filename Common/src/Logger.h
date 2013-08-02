/*---[Logger.h]------------------------------------------------m(._.)m-------*\
|
|  KnoRBA Common : Logging Facility
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
 * Header file for KnoRBA Logging facility. This file defines:
 *   - `org::knorba::common::log::level_t` enumeration
 *   - `org::knorba::common::Logger` class
 *   - `org::knorba::common::Logger::Channel` class 
 *
 */

#ifndef ORG_KNORBA_COMMON_LOGGER_H
#define ORG_KNORBA_COMMON_LOGGER_H

#include <ctime>
#include <cstring>
#include <sys/time.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#ifdef LOG
  #undef LOG
#endif

#define LOG_TO(LOGGER, LEVEL) LOGGER.log(LEVEL, __FILE__, __LINE__, __func__)
#define LOG(LEVEL) LOG_TO(Logger::getDefaultLogger(), LEVEL)
#define LOG_L1  LOG(org::knorba::common::Logger::L1)
#define LOG_L2  LOG(org::knorba::common::Logger::L2)
#define LOG_L3  LOG(org::knorba::common::Logger::L3)
#define LOG_WRN LOG(org::knorba::common::Logger::WRN)
#define LOG_ERR LOG(org::knorba::common::Logger::ERR)

#define __K_ENUMERAND(X)
#define __K_ENUMERATE_OVER_TYPES \
  __K_ENUMERAND(string&)\
  __K_ENUMERAND(const char*)\
  __K_ENUMERAND(const wchar_t*)\
  __K_ENUMERAND(char)\
  __K_ENUMERAND(int)\
  __K_ENUMERAND(long int)\
  __K_ENUMERAND(long long int)\
  __K_ENUMERAND(float)\
  __K_ENUMERAND(double)\
  __K_ENUMERAND(long double)
#undef __K_ENUMERAND

using namespace std;

namespace org {
namespace knorba {
namespace common {

typedef enum {
  EL
} logger_flag_t;

class Logger {

// -- NESTED DATA TYPES -------------------------------------------------------
public:
  typedef enum {
    ERR = 0,
    WRN = 1,
    L1  = 2,
    L2  = 3,
    L3  = 4,
  } level_t;

  class Channel {
  private:
    ostream* _os;
    bool     _closeOnDeconstruct;
    bool     _isSilent;
    level_t  _level;

  public:
    Channel(const string& fileName);
    Channel(ostream* os);
    ~Channel();
    inline ostream* getOStream() {
      return _os;
    }
    void setLevel(level_t level) {
      _level = level;
    }
    level_t getLevel() {
      return _level;
    }
    void setSilent(bool isSilent) {
      _isSilent = isSilent;
    }
    bool isSilent() {
      return _isSilent;
    }
  };

  class Stream {
  private:
    level_t _level;
    vector<Channel*>& _channels;

  public:
    Stream(level_t level, vector<Channel*>& channels)
      : _level(level), _channels(channels)
    {
      // Empty
    }
    #define __K_ENUMERAND(X) Stream& operator<<(X a);
    __K_ENUMERATE_OVER_TYPES
    #undef __K_ENUMERAND
    void operator<<(logger_flag_t f);
  };
  
// -- END -- NESTED DATA TYPES ------------------------------------------------

private:
  static Logger   _defaultLogger;
  static Channel* _defaultChannel;

  string            _name;
  vector<Channel*>  _channels;

  static void printTime(Stream& stream);

public:
  Logger(string name);
  ~Logger();

  inline static Logger& getDefaultLogger() {
    return _defaultLogger;
  }

  inline static Channel& getDefaultChannel() {
    return *_defaultChannel;
  }

  Channel* addChannel(ostream* os);
  Channel* addChannel(const string& fileName);
  void removeAllChannels();

  Stream& log(level_t level, const char fileName[], int lineNumebr, 
              const char functionName[]);

  Stream& log(level_t level);
};

} // namespace common
} // namespace knorba
} // namespace org

#endif
