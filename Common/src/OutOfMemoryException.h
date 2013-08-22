#ifndef ORG_KNORBA_COMMON_OUTOFMEMORYEXCEPTION_H
#define ORG_KNORBA_COMMON_OUTOFMEMORYEXCEPTION_H 

#include "MemoryException.h"

namespace org {
namespace knorba {
namespace common {

class OutOfMemoryException : public MemoryException {
public:
  OutOfMemoryException(string message)
    : MemoryException(message)
  {
    // Nothing
  }
};

} // namespace common
} // namespace knorba
} // namespace org

#endif
