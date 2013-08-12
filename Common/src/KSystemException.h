#ifndef ORG_KNORBA_COMMON_EXCEPTION_H
#define ORG_KNORBA_COMMON_EXCEPTION_H

#include "Streamer.h"

#include <ostream>

using namespace std;

namespace org {
namespace knorba {
namespace common {

class KSystemException : public Streamer {
private:
  string _message;

public:
  KSystemException(string message);
  const string& getMessage() {
    return _message;
  }

  virtual void printToStream(ostream& os) {
    os << "Exception[message: \"" << _message << "\"]";
  }
};

} // namespace common
} // namespace knorba
} // namespace org

#endif
