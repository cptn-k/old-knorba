#ifndef ORG_KNORBA_COMMON_NULLPOINTEREXCEPTION_H
#define ORG_KNORBA_COMMON_NULLPOINTEREXCEPTION_H   

#include "InvalidPointerException.h"

namespace org {
namespace knorba {
namespace common {

class NullPointerException : public InvalidPointerException {
public:
  NullPointerException(string message)
    : InvalidPointerException(message)
  {
    setName("NullPointerException");
  }
};

} // namespace common
} // namespace knorba
} // namespace org

#endif
