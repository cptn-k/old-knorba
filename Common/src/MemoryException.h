#ifndef ORG_KNORBA_COMMON_MEMORYEXCEPTION_H
#define ORG_KNORBA_COMMON_MEMORYEXCEPTION_H 

#include "KSystemException.h"

namespace org {
namespace knorba {
namespace common {

class MemoryException : public KSystemException {
protected:
  
public:
  MemoryException(string message)
    : KSystemException(message)
  {
    setName("MemoryException");
  }
};

} // namespace common
} // namespace knorba
} // namespace org

#endif
