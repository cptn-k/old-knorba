#ifndef ORG_KNORBA_COMMON_TYPE_BOOL_H
#define ORG_KNORBA_COMMON_TYPE_BOOL_H 

#include <ostream>

#include "Streamer.h"

using namespace std;

namespace org {
namespace knorba {
namespace common {
  
class Bool : public Streamer {
private:
  double _value;

public:
  Bool(bool value)
    : _value(value)
  {
    // Nothing
  }

  bool get() {
    return _value;
  }

  virtual void printToStream(ostream& os) const {
    os << toString(_value);
  }

  static string toString(bool value) {
    if(value)
      return "true";
    return "false";
  }
};

} // common
} // knorba
} // org

#endif
