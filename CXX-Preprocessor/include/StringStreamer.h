#ifndef ORG_KNORBA_COMMON_STIRNGSTREAMER
#define ORG_KNORBA_COMMON_STRINGSTREAMER

#include <sstream>
#include <string>

using namespace std;

namespace org {
namespace knorba {
namespace common {

class StringStreamer {
public:
  virtual void printToStringStream(stringstream& stream) = 0;
  string toString();
};

} // common
} // knorba
} // org

#endif
