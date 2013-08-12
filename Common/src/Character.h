#ifndef ORG_KNORBA_COMMON_TYPE_CHARACTER_H
#define ORG_KNORBA_COMMON_TYPE_CHARACTER_H 

#include <ostream>

#include "Streamer.h"

using namespace std;

namespace org {
namespace knorba {
namespace common {
  
class Character : Streamer {
public:
  typedef unsigned int qchar_t;

private:
  qchar_t _value;

public:
  Character(char value)
  {
    _value = (qchar_t)value;
  }

  Character(wchar_t value) {
    _value = (qchar_t)value;
  }

  virtual void pritnToStream(ostream& os) {
    os << _value;
  }
};

} // common
} // knorba
} // org

#endif
