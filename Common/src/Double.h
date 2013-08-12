#ifndef ORG_KNORBA_COMMON_TYPE_INTEGER_H
#define ORG_KNORBA_COMMON_TYPE_INTEGER_H 

#include <ostream>

#include "Streamer.h"

using namespace std;

namespace org {
namespace knorba {
namespace common {
  
class Integer : Streamer {
private:
  int _value;

public:
  Integer(int value)
    : _value(value)
  {
    // Nothing
  }

  virtual void pritnToStream(ostream& os) {
    os << _value;
  }
};

} // common
} // knorba
} // org

#endif
