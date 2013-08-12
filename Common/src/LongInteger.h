#ifndef ORG_KNORBA_COMMON_TYPE_LONGINTEGER_H
#define ORG_KNORBA_COMMON_TYPE_LONGINTEGER_H 

#include <ostream>

#include "Streamer.h"

using namespace std;

namespace org {
namespace knorba {
namespace common {
  
class LongInteger : Streamer {
private:
  long int _value;

public:
  Integer(long int value)
    : _value(value)
  {
    // Nothing
  }

  void pritnToStream(ostream& os) {
    os << _value;
  }
}

} // common
} // knorba
} // org

#endif
