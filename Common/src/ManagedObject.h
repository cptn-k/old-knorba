#ifndef ORG_KNORBA_COMMON_MANAGEDOBJECT_H
#define ORG_KNORBA_COMMON_MANAGEDOBJECT_H

#include "common.h"
#include "ObjectPool.h"
#include "PtrDecl.h"
#include "Streamer.h"

namespace org {
namespace knorba {
namespace common {

template<typename T>
class Ptr;

class ManagedObject {
public:
  ManagedObject();
  virtual ~ManagedObject();
};

} // common
} // knorba
} // org

#endif
