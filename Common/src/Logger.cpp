/*---[Logger.cpp]---------------------------------------------m(._.)m--------*\
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
 * Implementation of KnoRBA Logging Facility classes
 *   - `org::knorba::common::Logger`
 *   - `org::knorba::common::Logger::Channel`
 *   - `org::knorba::common::Logger::Stream`
 *
 */

#include "Logger.h"
#include "common.h"

namespace org {
namespace knorba {
namespace common {

// -- Logger::Channel ----------------------------------------------------------

Logger::Channel::Channel(const string& fileName) 
: _closeOnDeconstruct(true), 
  _isSilent(false), 
  _level(L3)
{
  ofstream* ofs = new ofstream();
  ofs->open(fileName.c_str(), ofstream::out | ofstream::app);
  _os = ofs;
}

Logger::Channel::Channel(ostream* os) 
: _os(os), 
  _closeOnDeconstruct(false), 
  _isSilent(false), 
  _level(L3)
{
  // Empty
}

Logger::Channel::~Channel() {
  if(_closeOnDeconstruct) {
    ((ofstream*)_os)->close();
    delete _os;
  }
}


// -- Logger::Stream -----------------------------------------------------------

#define __K_ENUMERAND(X) Logger::Stream& Logger::Stream::operator<<(X a) {\
  for(vector<Channel*>::iterator it = _channels.begin();\
      it != _channels.end(); it++)\
  {\
    Channel* ch = *it;\
    if(ch->isSilent() || ch->getLevel() < _level)\
      continue;\
    (*(ch->getOStream())) << a;\
  }\
  return *this;\
}

__K_ENUMERATE_OVER_TYPES

#undef __K_ENUMERAND

void Logger::Stream::operator<<(logger_flag_t f) {
  for(vector<Channel*>::iterator it = _channels.begin();
      it != _channels.end(); it++)
  {
    Channel* ch = *it;
    if(ch->isSilent() || ch->getLevel() < _level)
      continue;
    (*(ch->getOStream())) << endl;
  }
  delete this;
}

// -- Logger -------------------------------------------------------------------

Logger Logger::_defaultLogger = Logger("");
Logger::Channel* Logger::_defaultChannel = _defaultLogger.addChannel(&cerr);

void Logger::printTime(Logger::Stream& stream) {
  struct timeval tv;
  time_t t;
  
  gettimeofday(&tv, NULL);

  struct tm* timeInfo = localtime(&tv.tv_sec);

  static const int BUFFER_SIZE = 10;
  char buffer[BUFFER_SIZE];
  strftime(buffer, BUFFER_SIZE, "%H:%M:%S.", timeInfo);
  
  stream << buffer << tv.tv_usec;
}

Logger::Logger(string name) {
  _name = name;
}

Logger::~Logger() {
  removeAllChannels();
}

Logger::Channel* Logger::addChannel(ostream* os) {
  Channel* ch = new Channel(os);
  _channels.push_back(ch);
  return ch;
}

Logger::Channel* Logger::addChannel(const string& fileName) {
  Channel* ch = new Channel(fileName);
  _channels.push_back(ch);
  return ch;
}

void Logger::removeAllChannels() {
  for(vector<Channel*>::iterator it = _channels.begin();
      it != _channels.end(); it++)
  {
    Channel* ch = *it;
    delete ch;
  }
  _channels.clear();
}

Logger::Stream& Logger::log(level_t level, const char fileName[], 
                    int lineNumber, const char functionName[])
{
  const char* theName = strrchr(fileName, K_PATH_SEPARATOR_CH) + 1;
  Logger::Stream& res = (log(level) << '[' << functionName << '@' << theName 
                                     << ':' << lineNumber << "] ");
  return res;
}

Logger::Stream& Logger::log(level_t level) {
  Stream* stream = new Stream(level, _channels);
  printTime(*stream);
  *stream << " ";
  return *stream;
}

#undef __K_ENUMERATE_OVER_TYEPS

} // namespace common
} // namespace knorba
} // namespace org
