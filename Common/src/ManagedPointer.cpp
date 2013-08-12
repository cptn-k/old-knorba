
#include "ManagedPointer.h"

namespace org {
namespace knorba {
namespace common {

static void ManagedPointer::clearGarbage() {
#ifndef K_FAST_MANAGED_POINTERS
  for(int i = _allManagedPointers.size() - 1; i >= 0; i--) {
    if(_allManagedPointers[i]._retainCount == 0) {
      _allManagedPointers[i].release();
    }
  }
#endif
}

static void ManagedPointer::clearAllManagedPointers() {
#ifndef K_FAST_MANAGED_POINTERS
  for(int i = _allManagedPointers.size() - 1; i >= 0; i--) {
    if(_allManagedPointers[i]._obj != 0) {
      delete _allManagedPointers[i]._obj;
    }
  }
  _allManagedPointers.clear();
#endif
}

} // common
} // knorba
} // org

