#ifndef ORG_KNORBA_COMMON_INDEXOUTOFBOUNDEXCEPTION_H
#define ORG_KNORBA_COMMON_INDEXOUTOFBOUNDEXCEPTION_H

#include "MemoryException.h"

namespace org {
namespace knorba {
namespace common {

class IndexOutOfBoundException : public MemoryException {
public:
  IndexOutOfBoundException(string message)
    : MemoryException(message)
  {
    // Nothing
  }
};

} // namespace common
} // namespace knorba
} // namespace org

#endif
