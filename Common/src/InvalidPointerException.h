#ifndef ORG_KNORBA_COMMON_INVALIDPOINTEREXCEPTION_H
#define ORG_KNORBA_COMMON_INVALIDPOINTEREXCEPTION_H
   
#include "MemoryException.h"

namespace org {
namespace knorba {
namespace common {

class InvalidPointerException : public MemoryException {
public:
  InvalidPointerException(string message)
    : MemoryException(message)
  {
    setName("InvalidPointerException");
  }
};

} // namespace common
} // namespace knorba
} // namespace org

#endif
