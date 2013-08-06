/*---[Logger.h]------------------------------------------------m(._.)m-------*\
|
|  KnoRBA Common / Logging Facility
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
 *   - `org::knorba::common::logger_flag_t` enumeration
 *   - `org::knorba::common::Logger` class
 *   - `org::knorba::common::Logger::level_t` enumeration
 *   - `org::knorba::common::Logger::Channel` class 
 *   - `org::knorba::common::Logger::Stream` class
 *
 */

#ifndef ORG_KNORBA_COMMON_LOGGER_H
#define ORG_KNORBA_COMMON_LOGGER_H

#include "common.h"

#include <ctime>
#include <cstring>
#ifdef K_UNIX
#  include <sys/time.h>
#else
#  error "Only UNIX/Linux is supported"
#endif

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "Streamer.h"

#ifdef LOG
  #undef LOG
#endif

/// \cond
#define LOG_TO(LOGGER, LEVEL) LOGGER.log(LEVEL, __FILE__, __LINE__, __func__)
#define _K__LOG(LEVEL) LOG_TO((*Logger::getDefaultLogger()), LEVEL) 
#define LOG_ERR _K__LOG(::org::knorba::common::Logger::ERR)
#define LOG_WRN _K__LOG(::org::knorba::common::Logger::WRN)
#define LOG_L1  _K__LOG(::org::knorba::common::Logger::L1)
#define LOG_L2  _K__LOG(::org::knorba::common::Logger::L2)
#define LOG_L3  _K__LOG(::org::knorba::common::Logger::L3)
#define LOG LOG_L3

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
  __K_ENUMERAND(long double)\
  __K_ENUMERAND(Streamer&)
#undef __K_ENUMERAND
/// \endcond

using namespace std;

namespace org {
namespace knorba {
namespace common {

/** Special flags that can be send to logger stream. */
typedef enum {
  /**
   * "EndLog" flag: causes the stream to be closed and the resources consumed
   * by it to be freed.
   */
  EL
} logger_flag_t;


/**
 * The stream-based, multi-level, and multi-channel KnoRBA utility class for
 * logging.
 *
 * To use this utility, a logger stream should be initialized by calling either
 * one of the following macros:
 *
 *   - `LOG` - Creates a L3 logger stream on default logger.
 *   - `LOG_ERR`, `LOG_WRN`, `LOG_L1`, `LOG_L2`, and `LOG_L3` - Create a 
 *     logger stream with the desired level on the default logger.
 *   - `LOG_TO(LOGGER, LEVEL)` - Creates a stream on the given `LOGGER` with
 *     the given level.
 *
 * A logger stream can accept any primitive data type like `int`, `char` and
 * `float`, plus c-style null-terminated strings, and C++ `string` objects,
 * and any subclass of `Streamer` class.
 *
 * Each logger stream should be closed with `EL` flag.
 *
 * Example:
 *
 *     LOG << EL; // prints the time, line, file name, and function name.
 *
 *     if(n > 3) {
 *       LOG_WRN << "n=" << n << " is larger than 3" << EL;
 *     }
 *
 * Each log stream will be echoed in all available channels. Use 
 * `addChannel(ostream*)` and `addChannel(const string&)` to 
 * add `ostream`-based or file-based channels. The logger output will
 * automatically include a timestamp, file name, line number, and function 
 * name.
 *
 * There is a static default logger that can be obtained via
 * `Logger::getDefaultLogger()`. Default logger has one default channel that
 * prints to system's `err` console. This channel can be acessed by calling
 * `Logger::getDefaultChannel()`. 
 *
 * Each log message is assigned with a `severity level`. It is possible to
 * filter the the output of each channel to only display logs with severity
 * higher than a given level, using `Logger::Channel::setLevel(Logger::level_t)` 
 * function. There are 5 severity levels, ERR, WRN, L1, L2, and L3, in order 
 * from the highest to the lowest.
 *
 * For example, the following code causes the logs with severity level of L1
 * or higher to be printed to the default channel.
 *
 *     Logger::getDefaultChannel()->setLevel(Logger::L1).
 * 
 * To mute/unmute a channel use `Logger::Channel::setSilent(bool)` function.
 */
class Logger {

// -- NESTED DATA TYPES -------------------------------------------------------
public:
  /** Severity level */
  typedef enum {
    ERR = 0, /**< Error severity level    */
    WRN = 1, /**< Warning severity level  */
    L1  = 2, /**< Severity lower than WRN but higher than L2 */
    L2  = 3, /**< Severity lower than L1 but higher than L3  */
    L3  = 4, /**< The lowest severity level */
  } level_t;

  /** Logger channel */
  class Channel {
  private:
    ostream* _os;
    bool     _closeOnDeconstruct;
    bool     _isSilent;
    level_t  _level;

  public:
    /**
     * Constructs a channel that outputs to a file with the given name. If
     * the file does not exist, it will be created. If the file does exists,
     * the new logs will be appended to it.
     *
     * @param fileName the name of the file to use. 
     */
    Channel(const string& fileName);

    /**
     * Constructs a channel that outputs to the given stream.
     *
     * @param os the output stream to use.
     */
    Channel(ostream* os);

    /**
     * It this is a file channel, the file is closed before this object is
     * deconstructed.
     */
    ~Channel();

    /** @return the output stream associated with this channel */
    inline ostream* getOStream() {
      return _os;
    }

    /**
     * Set severity filtering level to the given values. The logs with severity
     * lower than the given value will be ignored.
     *
     * @param level
     */
    void setLevel(level_t level) {
      _level = level;
    }

    /** @return the severity filtering level */ 
    level_t getLevel() {
      return _level;
    }

    /**
     * Mutes/unmutes this channel.
     *
     * @param isSilent If `true`, the channel will be muted (all logs will be
     *                 ignored. If `false` the channel will be unmuted.
     */
    void setSilent(bool isSilent) {
      _isSilent = isSilent;
    }

    /** @return `true` of the channel is muted, otherwise `false`. */
    bool isSilent() {
      return _isSilent;
    }
  };

  class Stream {
  private:
    level_t _level;
    vector<Channel*>& _channels;

  public:
  /// \cond
    Stream(level_t level, vector<Channel*>& channels)
      : _level(level), _channels(channels)
    {
      // Empty
    }
    #define __K_ENUMERAND(X) Stream& operator<<(X a);
    __K_ENUMERATE_OVER_TYPES
    #undef __K_ENUMERAND
    void operator<<(logger_flag_t f);
  /// \endcond
  };

// -- END -- NESTED DATA TYPES ------------------------------------------------

private:
  static Logger   _defaultLogger;
  static Channel* _defaultChannel;

  string            _name;
  vector<Channel*>  _channels;

  static void printTime(Stream& stream);

public:
  /** 
   * Created a logger with the given name.
   *
   * @param name
   */
  Logger(string name);

  /** Destroys this object and all of its logging channels. */
  ~Logger();

  /** @return a pointer to the default logger */
  inline static Logger* getDefaultLogger() {
    return &_defaultLogger;
  }

  /** @return a pointer to the default channel of the default logger */
  inline static Channel* getDefaultChannel() {
    return _defaultChannel;
  }
  
  /**
   * Adds a channel to this logger that outputs to the given stream.
   *
   * @param os The output stream to print use.
   * @return pointer to the newly added channel.
   */
  Channel* addChannel(ostream* os);

  /**
   * Adds a channel to this logger than outputs to a file with the given name.
   * If the file does not exist, if will be created; otherwise, the new loggs
   * will be appended to it.
   *
   * @param fileName The name of the file to be used.
   * @return pointer to the newly added channel.
   */
  Channel* addChannel(const string& fileName);

  /** Removes all the channels. */
  void removeAllChannels();

  /// \cond

  /**
   * Creates a logger stream with the given level and prints the given
   * values for file name, line number and function name to it.
   *
   * @param level
   * @param fileName
   * @param lineNumber
   * @param functionName
   * @return the reference to the newly created logger stream.
   */
  Stream& log(level_t level, const char fileName[], int lineNumebr, 
              const char functionName[]);

  /**
   * Creates a new logger stream with the given level.
   */
  Stream& log(level_t level);

  /// \endcond
};

} // namespace common
} // namespace knorba
} // namespace org

#endif
